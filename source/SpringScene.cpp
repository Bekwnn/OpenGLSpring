#include "SpringScene.hpp"
#include <atlas/core/GLFW.hpp>
#include <atlas/core/Log.hpp>
#include <atlas/core/Macros.hpp>

SpringScene::SpringScene() :
	mIsPlaying(false),
	mLastTime(0.0)
{
	glEnable(GL_DEPTH_TEST);

	USING_ATLAS_MATH_NS;
	mCube = new Cube(Vector(0.f, 0.f, 10.f), Vector(2.f, 2.f, 2.f));
	mCube2 = new Cube(Vector(0.f, 0.f, 5.f), Vector(2.f, 2.f, 2.f));

	mPistonSpring = PistonSpring(2.f, 1.f, 0.01f);
	mAngularSpring = AngularSpring(Vector(0.f, 1.f, 0.f), 1.0f, 0.f);
}

SpringScene::~SpringScene()
{ }

void SpringScene::mousePressEvent(int button, int action, int modifiers,
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

void SpringScene::mouseMoveEvent(double xPos, double yPos)
{
	if (mIsDragging)
	{
		mCamera.mouseDrag(atlas::math::Point2(xPos, yPos));
	}
}

void SpringScene::keyPressEvent(int key, int scancode, int action, int mods)
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
			mCube->resetGeometry();
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
			}

		default:
			break;
		}
	}
}

void SpringScene::screenResizeEvent(int width, int height)
{
	glViewport(0, 0, width, height);
	mProjection = glm::perspective(glm::radians(45.0),
		(double)width / height, 1.0, 1000.0);
}

void SpringScene::renderScene()
{
	float grey = 161.0f / 255.0f;
	glClearColor(grey, grey, grey, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	mView = mCamera.getCameraMatrix();
	mGrid.renderGeometry(mProjection, mView);
	mCube->renderGeometry(mProjection, mView);
	mCube2->renderGeometry(mProjection, mView);
}

void SpringScene::updateScene(double time)
{
	if (mIsPlaying)
	{
		// Handle the timing stuff here.
		mTime.deltaTime = (float)time - mTime.currentTime;
		mTime.totalTime += (float)time;
		mTime.currentTime = (float)time;

		//get the spring force and apply it to the object (letting mass = 1.0)
		USING_ATLAS_MATH_NS;
		Vector springForce = mPistonSpring.GetForce(mCube->getPosition(), Vector(0.f, 0.f, 0.f), mCube->objController->getVelocity(), Vector(0.f, 0.f, 0.f));
		mCube->objController->addVelocity(springForce);

		Vector angularForce = mAngularSpring.GetForce(mCube2->getPosition(), Vector(0.f, 0.f, 0.f), mCube->objController->getVelocity());
		mCube2->objController->addVelocity(angularForce);

		//cube will spiral further and further away, calculate future position and offset to prevent that:
		Vector futurePos = mCube2->getPosition()
			+ mCube2->objController->getVelocity()*mTime.deltaTime
			+ mCube2->objController->getAccel()*mTime.deltaTime*mTime.deltaTime
			+ mCube2->objController->getMovement();
		mCube2->objController->addMove(normalize(futurePos)* (length(mCube2->getPosition()) - length(futurePos)));

		printf("piston cube pos: (%f, %f, %f)\n", mCube->getPosition().x, mCube->getPosition().y, mCube->getPosition().z);
		printf("angular cube pos: (%f, %f, %f)\n", mCube2->getPosition().x, mCube2->getPosition().y, mCube2->getPosition().z);

		// Tell our cube to update itself.
		mCube->updateGeometry(mTime);
		mCube2->updateGeometry(mTime);
	}
}