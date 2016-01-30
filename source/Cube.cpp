#include "Cube.hpp"
#include "ShaderPaths.hpp"

#include <atlas/gl/Shader.hpp>
#include <atlas/core/Macros.hpp>

Cube::Cube(atlas::math::Vector pos, atlas::math::Vector size)
{
	USING_ATLAS_GL_NS;
	USING_ATLAS_MATH_NS;

	mPosition = pos;
	objController = new ObjectController();

	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	Vector vertices[8];
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			for (int z = 0; z < 2; z++)
			{
				vertices[x * 4 + y * 2 + z].x = -size.x / 2.f + size.x*x;
				vertices[x * 4 + y * 2 + z].y = -size.y / 2.f + size.y*y;
				vertices[x * 4 + y * 2 + z].z = -size.z / 2.f + size.z*z;
			}
		}
	}

	// Set the vertices.
	static const Vector vertexBufferData[] = {
		vertices[0],
		vertices[1],
		vertices[3], //1
		vertices[6],
		vertices[0],
		vertices[2], //2
		vertices[5],
		vertices[0],
		vertices[4], //3
		vertices[6],
		vertices[4],
		vertices[0], //4
		vertices[0],
		vertices[3],
		vertices[2], //5
		vertices[5],
		vertices[1],
		vertices[0], //6
		vertices[3],
		vertices[1],
		vertices[5], //7
		vertices[7],
		vertices[4],
		vertices[6], //8
		vertices[4],
		vertices[7],
		vertices[5], //9
		vertices[7],
		vertices[6],
		vertices[2], //10
		vertices[7],
		vertices[2],
		vertices[3], //11
		vertices[7],
		vertices[3],
		vertices[5] //12
	};

	// Now generate and bind the vertex buffer.
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData) * sizeof(Vector),
		vertexBufferData, GL_STATIC_DRAW);

	std::string shaderDir = generated::ShaderPaths::getShaderDirectory();

	std::vector<ShaderInfo> shaders
	{
		ShaderInfo{ GL_VERTEX_SHADER, shaderDir + "cloth.vs.glsl" },
		ShaderInfo{ GL_FRAGMENT_SHADER, shaderDir + "cloth.fs.glsl" }
	};

	mModel = glm::translate(Matrix4(1.0f), mPosition);

	mShaders.push_back(ShaderPointer(new Shader));
	mShaders[0]->compileShaders(shaders);
	mShaders[0]->linkShaders();

	// Grab the uniform variables.
	GLuint mvpMat = mShaders[0]->getUniformVariable("mvpMat");
	mUniforms.insert(UniformKey("mvpMat", mvpMat));

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	mShaders[0]->disableShaders();
}

Cube::~Cube()
{
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(1, &mVertexBuffer);
	delete objController;
}

void Cube::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view)
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

	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

	glDisableVertexAttribArray(0);

	mShaders[0]->disableShaders();
}

void Cube::updateGeometry(atlas::utils::Time const& t)
{
	USING_ATLAS_MATH_NS;

	if (objController == nullptr)
		return;

	if (objController->bIsStopped)
		return;
	
	objController->addVelocity(Vector(2.f, 0.f, 0.f)* t.deltaTime);

	mPosition = mPosition + objController->updateController(t);

	mModel = glm::translate(Matrix4(1.0f), mPosition);	
}

void Cube::resetGeometry()
{
	mPosition = atlas::math::Vector(0.f, 0.f, 0.f);

	if (objController != nullptr)
	{
		objController->resetAccel();
		objController->resetVelocity();
	}
}