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

bool isMouseLocked = false;
vec3 lightPos = {0,2.5,-5};
float shininess = 32;
float specularStrength = 1;
void SetupInput()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // seteaza cursorul lockat pe window
	Input::Init(window);
	inputThread = new std::thread([&] {
		while (!glfwWindowShouldClose(window))
		{
			if (glfwGetKey(window, GLFW_KEY_SPACE))
			{
				if (isMouseLocked == false)
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED), isMouseLocked = true;
				else
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL), isMouseLocked = false;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
				cam->position.y += camSpeed;
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
				cam->position.y -= camSpeed;
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

	float far = 100;
	float near = 0.1;
	float fov = pi / 4;
	cam = new Camera(glm::perspective(fov, ((float)Window_Width / Window_Height), near, far), glm::vec3(0, 0, -10));

	cube = new CubeModel();
	cube->color = vec4(1);

	sh = new Shader(SHADER_PATH "DefaultMesh.shader");
	model = new Model(MODEL_PATH "Backpack/backpack.obj",sh);
	gc(glEnable(GL_DEPTH_TEST));
	gc(glDepthFunc(GL_LESS));

}

void GuiRender()
{
	ImGui::SliderFloat3("Light Position", &lightPos.x, -5, 5);
	ImGui::SliderFloat("Shininess", &shininess, 0, 200);
	ImGui::SliderFloat("SpecularStrength", &specularStrength, 0, 1);
}

void RenderingEngine::Render()
{
	lightPos = mat3(rotate(mat4(1), (float)pi / 160, vec3(0, 1, 0))) * lightPos;
	
	cube->viewMat = translate(mat4(1), lightPos);
	cube->Draw(*cam);
	sh->SetUniform3f("u_lightPos", lightPos);
	sh->SetUniform1f("u_shininess", shininess);
	sh->SetUniform1f("u_specularStrength", specularStrength);
	
	model->Draw(*cam);

	GuiRender();
}

void RenderingEngine::Update()
{
}

void RenderingEngine::Terminate()
{
	inputThread->join();

}
