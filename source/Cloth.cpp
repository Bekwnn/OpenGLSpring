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

	//build vertices
	//(reason I'm using std::vector is that VS compiler doesn't allow for arrays undefined at compile time)
	Vector topleftpos = pos - Vector((xverts-1) / 2.f, -(yverts-1) / 2.f, 0.f);

	std::vector<std::vector<Vector>> vertices; // this is a fun type.
	vertices.resize(xverts);
	for (int i = 0; i < xverts; i++)
	{
		vertices[i].resize(yverts);
		for (int j = 0; j < yverts; j++)
		{
			vertices[i][j] = topleftpos + Vector(i, -j, 0.f);
			printf("vertex %d: (%f, %f, %f)\n", i*yverts+j, vertices[i][j].x, vertices[i][j].y, vertices[i][j].z);
		}
	}


	std::cout << "vertices built" << std::endl;

	//build faces
	std::vector<Vector> vertexBufferData;
	vertexBufferData.resize(6 * (xverts - 1)*(yverts - 1));
	for (int i = 0; i < xverts-1; i++)
	{
		for (int j = 0; j < yverts-1; j++)
		{
			int indexoffset = 6 * (i*(yverts-1) + j);
			//triangle 1
			vertexBufferData[indexoffset] = vertices[i][j];
			vertexBufferData[indexoffset +1] = vertices[i+1][j];
			vertexBufferData[indexoffset +2] = vertices[i][j+1];
			//triangle 2
			vertexBufferData[indexoffset +3] = vertices[i][j+1];
			vertexBufferData[indexoffset +4] = vertices[i+1][j];
			vertexBufferData[indexoffset +5] = vertices[i+1][j+1];
			printf("%d\n", indexoffset);
		}
	}

	std::cout << "faces built" << std::endl;

	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferData.size()*sizeof(Vector), &vertexBufferData[0], GL_STATIC_DRAW);

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
}

Cloth::~Cloth()
{
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(1, &mVertexBuffer);
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

	glDrawArrays(GL_TRIANGLES, 0, 2*(xverts-1)*(yverts-1) * 3);

	glDisableVertexAttribArray(0);

	mShaders[0]->disableShaders();
}

void Cloth::updateGeometry(atlas::utils::Time const & t)
{

}

void Cloth::resetGeometry()
{
}
