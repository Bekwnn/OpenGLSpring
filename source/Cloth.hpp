#ifndef CLOTH_HPP
#define CLOTH_HPP

#pragma once

#include <atlas/utils/Geometry.hpp>

class Cloth : public atlas::utils::Geometry
{
public:
	Cloth();
	~Cloth();

	void renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view) override;

private:
	GLuint mVao;
	GLuint mBuffer;
};

#endif
