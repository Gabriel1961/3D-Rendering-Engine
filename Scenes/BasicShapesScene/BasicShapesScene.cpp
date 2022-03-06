#include "BasicShapesScene.h"
BasicShapesScene basicShapeScene;
int icoLod = 0;
Shader* icoShader = 0;
void BasicShapesScene::Start(GLFWwindow* win)
{
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // seteaza cursorul lockat pe window

	float far = 100;
	float near = 0.1;
	float fov = pi / 4;
	cam = new Camera(glm::perspective(fov, ((float)Window_Width / Window_Height), near, far), glm::vec3(0, 0, 10), win);
	//cube = new Model(MODEL_PATH "Cube/Cube.obj",new Shader(SHADER_PATH "Viewport.shader"));
	icoShader = new Shader(SHADER_PATH "Viewport.shader");
	ico = new IcoSphereModel(1,icoShader);
}

void BasicShapesScene::Render()
{
	ico->Draw(*cam);
}

void BasicShapesScene::Update()
{
	static bool lastpress = 0, lastpress1 = 0;
	if (glfwGetKey(window, GLFW_KEY_O) && lastpress == 0)
	{
		lastpress = 1;
		delete ico;
		icoLod++;
		ico = new IcoSphereModel(icoLod, icoShader);
	}
	else lastpress = 0;

	if (glfwGetKey(window, GLFW_KEY_I) && lastpress1 == 0)
	{
		lastpress1 = 1;
		delete ico;
		icoLod--;
		ico = new IcoSphereModel(icoLod, icoShader);
	}
	else lastpress1 = 0;
	cam->UpdateInput(); 
}

void BasicShapesScene::Terminate()
{
	delete ico;
}
