#include "3DRenderingEngine.h"
#include "../OpenGLWrapper/Common.h"
#include "../ScreenSize.h"
#include "../../OpenGLWrapper/vendor/imgui/ImguiLib.h"
#include <fstream>
#include <thread>
#include <Input/Input.h>
#include <Mesh.h>
#include <Camera.h>
#include <Models/PreBuilt/CubeModel.h>
using namespace std;
using namespace glm;
const double camSpeed = -0.1;


thread* inputThread;
Camera* cam;
GLFWwindow* window;
CubeModel* cube;
void SetupInput()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // seteaza cursorul lockat pe window
	Input::Init(window);
	inputThread = new std::thread([&] {
		while (!glfwWindowShouldClose(window))
		{
			if (glfwGetKey(window, GLFW_KEY_SPACE))
				cam->position.y -= camSpeed;
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
				cam->position.y += camSpeed;
			if (glfwGetKey(window, GLFW_KEY_A))
			{
				cam->position.x -= cos(cam->rotation.x) * camSpeed;
				cam->position.z -= sin(cam->rotation.x) * camSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_D))
			{
				cam->position.x += cos(cam->rotation.x) * camSpeed;
				cam->position.z += sin(cam->rotation.x) * camSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_S))
			{
				cam->position.y -= sin(cam->rotation.y) * camSpeed;
				cam->position.x -= sin(cam->rotation.x) * camSpeed;
				cam->position.z += cos(cam->rotation.x) * camSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_W))
			{
				cam->position.y += sin(cam->rotation.y) * camSpeed;
				cam->position.x += sin(cam->rotation.x) * camSpeed;
				cam->position.z -= cos(cam->rotation.x) * camSpeed;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		});

	Input::Mouse::MouseMove += [](double speedX, double speedY) {
		if (abs(speedX) > 1 || abs(Input::Mouse::SpeedY) > 1)
			return;
		cam->rotation.x += speedX;
		cam->rotation.y += speedY;
		if (cam->rotation.y > pi / 2)
			cam->rotation.y = pi / 2;
		if (cam->rotation.y < -pi / 2)
			cam->rotation.y = -pi / 2;
	};
}
Model* model;
Shader* sh;
void RenderingEngine::Start(GLFWwindow* win)
{
	window = win;
	SetupInput();

	float far = 1000;
	float near = 0.1;
	float fov = pi / 4;
	cam = new Camera(glm::perspective(fov, ((float)Window_Width / Window_Height), near, far), glm::vec3(0, 0, -10));
	cube = new CubeModel();

	sh = new Shader(SHADER_PATH "DefaultMesh.shader");
	model = new Model(MODEL_PATH "Monkey2/Monkey2.obj",sh);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void RenderingEngine::Render()
{
	//cube->Draw(*cam);
	model->Draw(*cam);
}

void RenderingEngine::Update()
{
}

void RenderingEngine::Terminate()
{
	inputThread->join();
}
