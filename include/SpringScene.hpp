#ifndef SPRING_SCENE_HPP
#define SPRING_SCENE_HPP 

#pragma once

#include <atlas/utils/Scene.hpp>

#include "Cube.hpp"
#include "MayaCamera.hpp"
#include "Grid.hpp"
#include "PistonSpring.hpp"
#include "AngularSpring.hpp"

class SpringScene : public atlas::utils::Scene
{
public:
	SpringScene();
	~SpringScene();

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

	Cube* mCube;
	Cube* mCube2;
	PistonSpring mPistonSpring;
	AngularSpring mAngularSpring;
};

#endif

