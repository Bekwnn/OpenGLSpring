#ifndef CLOTH_GL_SCENE_HPP
#define CLOTH_GL_SCENE_HPP 

#pragma once

#include <atlas/utils/Scene.hpp>
#include "Cloth.hpp"

class ClothGLScene : public atlas::utils::Scene
{
public:
	ClothGLScene();
	~ClothGLScene();

	void renderScene() override;

private:
	Cloth mCloth;
};

#endif

