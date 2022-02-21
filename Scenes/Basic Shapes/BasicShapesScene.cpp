#include "BasicShapesScene.h"
/// TODO change the event for mouse move with a loop in the update **(Smoother cursor)
BasicShapesScene basicShapeScene;
void BasicShapesScene::Start(GLFWwindow* win)
{
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // seteaza cursorul lockat pe window

	float far = 100;
	float near = 0.1;
	float fov = pi / 4;
	cam = new Camera(glm::perspective(fov, ((float)Window_Width / Window_Height), near, far), glm::vec3(0, 0, -10), win);
	cube = new CubeModel();
}

void BasicShapesScene::Render()
{
	cube->Draw(*cam);
}

void BasicShapesScene::Update()
{
	cam->UpdateInput(); 
}

void BasicShapesScene::Terminate()
{
}
