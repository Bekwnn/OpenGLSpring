#include "ClothScene.hpp"
#include <atlas/core/GLFW.hpp>
#include <atlas/core/Macros.hpp>
#include <iostream>

ClothScene::ClothScene() :
	mIsPlaying(false),
	mLastTime(0.0)
{
	glEnable(GL_DEPTH_TEST);

	USING_ATLAS_MATH_NS;
	mCloth = new Cloth(Vector(0.f, 10.f, 0.f), 25, 25, TOPALL);

	mCamera.waypoints.push_back(Vector(40.f, 10.f, 40.f));
	mCamera.waypoints.push_back(Vector(-40.f, 20.f, 40.f));
	mCamera.waypoints.push_back(Vector(-40.f, 30.f, -40.f));
	mCamera.waypoints.push_back(Vector(40.f, 40.f, -40.f));
	
	for (int i = 0; i < mCamera.waypoints.size(); i++)
		mCamera.transitionDurations.push_back(5.f);

}

ClothScene::~ClothScene()
{

}

void ClothScene::renderScene()
{
	float grey = 161.0f / 255.0f;
	glClearColor(grey, grey, grey, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	mView = mCamera.getCameraMatrix();
	mGrid.renderGeometry(mProjection, mView);
	mCloth->renderGeometry(mProjection, mView);
}

void ClothScene::updateScene(double time)
{
	if (mIsPlaying)
	{
		// Handle the timing stuff here.
		mTime.deltaTime = (float)time - mTime.currentTime;
		mTime.totalTime += (float)time;
		mTime.currentTime = (float)time;

		mTime.deltaTime *= 2.0f;

		// Tell our cloth to update itself.
		mCloth->updateGeometry(mTime);
		mCamera.updateCameraRail(mTime);
	}
}

void ClothScene::keyPressEvent(int key, int scancode, int action, int mods)
{
	UNUSED(scancode);
	UNUSED(mods);

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_R:
			mCamera.resetCamera();
			break;

		case GLFW_KEY_C:
			mCloth->resetGeometry();
			break;

		case GLFW_KEY_SPACE:
			if (mIsPlaying)
			{
				mLastTime = glfwGetTime();
			}
			else
			{
				glfwSetTime(mLastTime);
				mIsPlaying = !mIsPlaying;
				mCamera.bIsPlaying = mIsPlaying;
			}

		default:
			break;
		}
	}
}

void ClothScene::mousePressEvent(int button, int action, int modifiers,
	double xPos, double yPos)
{
	USING_ATLAS_MATH_NS;

	if (button == GLFW_MOUSE_BUTTON_LEFT && modifiers == GLFW_MOD_ALT)
	{
		if (action == GLFW_PRESS)
		{
			mIsDragging = true;
			mCamera.mouseDown(Point2(xPos, yPos),
				MayaCamera::CameraMovements::TUMBLE);
		}
		else
		{
			mIsDragging = false;
			mCamera.mouseUp();
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && modifiers == GLFW_MOD_ALT)
	{
		if (action == GLFW_PRESS)
		{
			mIsDragging = true;
			mCamera.mouseDown(Point2(xPos, yPos),
				MayaCamera::CameraMovements::TRACK);
		}
		else
		{
			mIsDragging = false;
			mCamera.mouseUp();
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && modifiers == GLFW_MOD_ALT)
	{
		if (action == GLFW_PRESS)
		{
			// first click.
			mIsDragging = true;
			mCamera.mouseDown(Point2(xPos, yPos),
				MayaCamera::CameraMovements::DOLLY);
		}
		else
		{
			mIsDragging = false;
			mCamera.mouseUp();
		}
	}
	else if (action != GLFW_PRESS)
	{
		mIsDragging = false;
		mCamera.mouseUp();
	}
}

void ClothScene::mouseMoveEvent(double xPos, double yPos)
{
	if (mIsDragging)
	{
		mCamera.mouseDrag(atlas::math::Point2(xPos, yPos));
	}
}

void ClothScene::screenResizeEvent(int width, int height)
{
	glViewport(0, 0, width, height);
	mProjection = glm::perspective(glm::radians(45.0),
		(double)width / height, 1.0, 1000.0);
}