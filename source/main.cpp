#include <atlas/utils/Application.hpp>

#include "ClothScene.hpp"
#include "SpringScene.hpp"

int main()
{
	APPLICATION.createWindow(400, 400, "OpenGL");
	APPLICATION.addScene(new ClothScene);
	APPLICATION.runApplication();

	return 0;
}
