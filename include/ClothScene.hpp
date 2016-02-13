#ifndef CLOTH_GL_SCENE_HPP
#define CLOTH_GL_SCENE_HPP

#pragma once

#include <atlas/utils/Scene.hpp>

#include "Cloth.hpp"
#include "MayaCamera.hpp"
#include "Grid.hpp"

class ClothScene : public atlas::utils::Scene
{
public:
	ClothScene();
	~ClothScene();

	void renderScene() override;
	void updateScene(double time) override;

	void keyPressEvent(int key, int scancode, int action, int mods) override;
	void mousePressEvent(int button, int action, int modifiers, double xPos, double yPos) override;
	void mouseMoveEvent(double xPos, double yPos) override;

	void screenResizeEvent(int width, int height) override;

private:

	bool mIsPlaying;
	bool mIsDragging;
	double mLastTime;

	Grid mGrid;
	MayaCamera mCamera;
	Cloth* mCloth;
};

#endif

