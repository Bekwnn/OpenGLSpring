#include "Cloth.hpp"
#include "ShaderPaths.hpp"

#include <atlas/gl/Shader.hpp>
#include <atlas/core/Macros.hpp>

Cloth::Cloth()
{
	USING_ATLAS_GL_NS;

	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	GLfloat vertices[6][2] = {
		{ -0.90f, -0.90f }, // First triangle.
		{ 0.85f, -0.90f },
		{ -0.90f,  0.85f },
		{ 0.90f, -0.85f }, // Second triangle.
		{ 0.90f,  0.90f },
		{ -0.85f,  0.90f }
	};

	glGenBuffers(1, &mBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
	glDeleteBuffers(1, &mBuffer);
}

void Cloth::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view)
{
	UNUSED(projection);
	UNUSED(view);

	mShaders[0]->enableShaders();

	glBindVertexArray(mVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	mShaders[0]->disableShaders();
}
