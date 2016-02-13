#include "Cloth.hpp"
#include "ShaderPaths.hpp"
#include <iostream>

#include <atlas/gl/Shader.hpp>
#include <atlas/core/Macros.hpp>

Cloth::Cloth(atlas::math::Vector pos, int xverts, int yverts, EPinType pinType) :
	pinType(pinType),
	xverts(xverts),
	yverts(yverts)
{
	USING_ATLAS_GL_NS;
	USING_ATLAS_MATH_NS;

	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	std::cout << "cloth constructor" << std::endl;

	//build vertexData
	//(reason I'm using std::vector is that VS compiler doesn't allow for arrays undefined at compile time)
	Vector topleftpos = pos - Vector((xverts-1) / 2.f, -(yverts-1) / 2.f, 0.f);

	vertexData.resize(xverts);
	vertexVelocities.resize(xverts);

	for (int i = 0; i < xverts; i++)
	{
		vertexData[i].resize(yverts);
		vertexVelocities[i].resize(yverts);

		for (int j = 0; j < yverts; j++)
		{
			vertexVelocities[i][j] = Vector(0.f, 0.f, 0.f);
			vertexData[i][j] = topleftpos + Vector(i, -j, 0.f);
			printf("vertex %d: (%f, %f, %f)\n", i*yverts + j, vertexData[i][j].x, vertexData[i][j].y, vertexData[i][j].z);
		}
	}


	std::cout << "vertexData built" << std::endl;

	//build faces
	std::vector<Vector> vertexBufferData = BuildFaces();

	std::cout << "faces built" << std::endl;

	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferData.size()*sizeof(Vector), &vertexBufferData[0], GL_STREAM_DRAW);

	std::string shaderDir = generated::ShaderPaths::getShaderDirectory();

	std::vector<ShaderInfo> shaders
	{
		ShaderInfo{ GL_VERTEX_SHADER, shaderDir + "cloth.vs.glsl" },
		ShaderInfo{ GL_FRAGMENT_SHADER, shaderDir + "cloth.fs.glsl" }
	};

	mShaders.push_back(ShaderPointer(new Shader));

	mShaders[0]->compileShaders(shaders);

	mShaders[0]->linkShaders();

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	mShaders[0]->disableShaders();

	//set up springs
	structuralSpring = PistonSpring(1.f, 6.f, 1.0f);
	shearSpring = PistonSpring(sqrt(2.f), 6.f, 1.0f);
	bendSpring = PistonSpring(2.f, 6.f, 1.0f);
}

Cloth::~Cloth()
{
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(1, &mVertexBuffer);
}

std::vector<atlas::math::Vector> Cloth::BuildFaces()
{
	std::vector<atlas::math::Vector> vertexBufferData;
	vertexBufferData.resize(6 * (xverts - 1)*(yverts - 1));
	for (int i = 0; i < xverts - 1; i++)
	{
		for (int j = 0; j < yverts - 1; j++)
		{
			int indexoffset = 6 * (i*(yverts - 1) + j);
			//triangle 1
			vertexBufferData[indexoffset] = vertexData[i][j];
			vertexBufferData[indexoffset + 1] = vertexData[i + 1][j];
			vertexBufferData[indexoffset + 2] = vertexData[i][j + 1];
			//triangle 2
			vertexBufferData[indexoffset + 3] = vertexData[i][j + 1];
			vertexBufferData[indexoffset + 4] = vertexData[i + 1][j];
			vertexBufferData[indexoffset + 5] = vertexData[i + 1][j + 1];
		}
	}

	return vertexBufferData;
}

bool Cloth::SkipVert(int i, int j)
{
	switch (pinType)
	{
	case TOPLEFT:
		return (i == 0 && j == 0);
		break;
	case TOPLEFTRIGHT:
		return (j == 0 && (i == 0 || i == xverts - 1));
		break;
	case TOPALL:
		return (j == 0);
		break;
	case FOURCORNERS:
		return ((i == 0 && (j == 0 || j == yverts - 1)) || (i == xverts - 1 && (j == 0 || j == yverts - 1)));
		break;
	case NONE:
	default:
		return false;
	}
}

void Cloth::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view)
{
	UNUSED(projection);
	UNUSED(view);

	mShaders[0]->enableShaders();

	glBindVertexArray(mVao);

	auto mvpMat = projection * view * mModel;
	glUniformMatrix4fv(mUniforms["mvpMat"], 1, GL_FALSE, &mvpMat[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, 2 * (xverts - 1)*(yverts - 1) * 3);

	glDisableVertexAttribArray(0);

	mShaders[0]->disableShaders();
}

void Cloth::updateGeometry(atlas::utils::Time const & t)
{
	USING_ATLAS_MATH_NS;

	std::vector<std::vector<Vector>> sumForces; //store our update data and then add it after calculations (because of anchor velocity)
	sumForces.resize(xverts);

	//update vertex data
	for (int i = 0; i < xverts; i++)
	{
		sumForces[i].resize(yverts);

		for (int j = 0; j < yverts; j++)
		{
			if (SkipVert(i, j)) continue;

			sumForces[i][j] = Vector(0.f, 0.f, 0.f);

			//gravity
			sumForces[i][j] += Vector(0.f, -1.f, sinf(t.currentTime)); //arbitrary fake gravity constant

			//STRUCTURAL SPRINGS
			//left (i-1)
			if (i > 0)			sumForces[i][j] += GetClothForce(structuralSpring, i, j, -1, 0);
			//right (i+1)
			if (i < xverts-1)	sumForces[i][j] += GetClothForce(structuralSpring, i, j, 1, 0);
			//above (j-1)
			if (j > 0)			sumForces[i][j] += GetClothForce(structuralSpring, i, j, 0, -1);
			//below (j+1)
			if (j < yverts-1)	sumForces[i][j] += GetClothForce(structuralSpring, i, j, 0, 1);

			//SHEAR SPRINGS
			//above left
			if (i > 0 && j > 0)					sumForces[i][j] += GetClothForce(shearSpring, i, j, -1, -1);
			//above right
			if (i < xverts-1 && j > 0)			sumForces[i][j] += GetClothForce(shearSpring, i, j, 1, -1);
			//below left
			if (i > 0 && j < yverts-1)			sumForces[i][j] += GetClothForce(shearSpring, i, j, -1, 1);
			//below right
			if (i < xverts-1 && j < yverts-1)	sumForces[i][j] += GetClothForce(shearSpring, i, j, 1, 1);

			//BEND SPRINGS
			//left by two
			if (i > 1)			sumForces[i][j] += GetClothForce(bendSpring, i, j, -2, 0);
			//right by two
			if (i < xverts-2)	sumForces[i][j] += GetClothForce(bendSpring, i, j, 2, 0);
			//above by two
			if (j > 1)			sumForces[i][j] += GetClothForce(bendSpring, i, j, 0, -2);
			//below by two
			if (j < yverts-2)	sumForces[i][j] += GetClothForce(bendSpring, i, j, 0, 2);
		}
	}

	for (int i = 0; i < xverts; i++)
	{
		for (int j = 0; j < yverts; j++)
		{
			vertexVelocities[i][j] += sumForces[i][j] * t.deltaTime;
			vertexData[i][j] += vertexVelocities[i][j] * t.deltaTime;
		}
	}

	//build and render faces
	std::vector<atlas::math::Vector> vertexBufferData = BuildFaces();
	glBufferData(GL_ARRAY_BUFFER, vertexBufferData.size() * sizeof(atlas::math::Vector), &vertexBufferData[0], GL_STREAM_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 2 * (xverts - 1)*(yverts - 1) * 3);
}

//to save my poor wrists from typing this all out
atlas::math::Vector Cloth::GetClothForce(PistonSpring spring, int i, int j, int ioffsetOther, int joffsetOther)
{
	return spring.GetForce(vertexData[i][j], vertexData[i + ioffsetOther][j + joffsetOther], vertexVelocities[i][j], vertexVelocities[i + ioffsetOther][j + joffsetOther]);
}

void Cloth::resetGeometry()
{
}
