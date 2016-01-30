#ifndef CUBE_HPP
#define CUBE_HPP

#pragma once

#include <atlas/utils/Geometry.hpp>
#include <atlas/math/Vector.hpp>

#include "ObjectController.hpp"

class Cube : public atlas::utils::Geometry
{
public:
	Cube(atlas::math::Vector pos, atlas::math::Vector size);
	~Cube();

	atlas::math::Vector getPosition() { return mPosition; }

	void renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view) override;
	void updateGeometry(atlas::utils::Time const& t) override;
	void resetGeometry() override;

	ObjectController* objController; //optional physics component

private:
	GLuint mVao;
	GLuint mVertexBuffer;
	atlas::math::Vector mPosition;
};

#endif
