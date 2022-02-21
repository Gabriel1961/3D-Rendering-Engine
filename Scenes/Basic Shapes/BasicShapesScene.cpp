#include "BasicShapesScene.h"
#include "Common.h"
#include "../ScreenSize.h"
#include "../../OpenGLWrapper/vendor/imgui/ImguiLib.h"
#include <Input/Input.h>
#include <Mesh.h>
#include <Camera.h>
#include <Models/PreBuilt/CubeModel.h>
#include "./CubeMap/CubeMap.h"
#include "../Scenes/Scene.h"

Camera* cam;
CubeModel* cube;
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
}

void BasicShapesScene::Update()
{
}

void BasicShapesScene::Terminate()
{
}
