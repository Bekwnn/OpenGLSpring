#ifndef CLOTH_HPP
#define CLOTH_HPP

#pragma once

#include <atlas/utils/Geometry.hpp>

// sets which vertices are pinned in place
enum EPinType {
	TOPLEFT,
	TOPLEFTRIGHT,
	TOPALL,
	FOURCORNERS,
	NONE
};

class Cloth : public atlas::utils::Geometry
{
public:
	Cloth(atlas::math::Vector pos, int xverts, int yverts, EPinType pinType);
	~Cloth();

	void renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view) override;
	void updateGeometry(atlas::utils::Time const& t) override;
	void resetGeometry() override;

private:
	GLuint mVao;
	GLuint mVertexBuffer;
	EPinType pinType;
	int xverts;
	int yverts;
};

#endif
