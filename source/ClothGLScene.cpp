#include "ClothGLScene.hpp"

ClothGLScene::ClothGLScene()
{
	mProjection = atlas::math::Matrix4(1.0f);
	mView = atlas::math::Matrix4(1.0f);
}

ClothGLScene::~ClothGLScene()
{ }

void ClothGLScene::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	mCloth.renderGeometry(mProjection, mView);
}
