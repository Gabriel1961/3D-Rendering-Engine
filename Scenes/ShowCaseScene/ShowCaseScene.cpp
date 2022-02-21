#include "./ShowCaseScene.h"
#include "../OpenGLWrapper/Common.h"
#include "../ScreenSize.h"
#include "../../OpenGLWrapper/vendor/imgui/ImguiLib.h"
#include <fstream>
#include <thread>
#include <Input/Input.h>
#include <Mesh.h>
#include <Camera.h>
#include <Models/PreBuilt/CubeModel.h>
#include "./CubeMap/CubeMap.h"
#include "../Scenes/Scene.h"
using namespace std;
using namespace glm;
ShowCaseScene showCaseScene;
Camera* cam;
CubeMap* cubeMap;
CubeModel* cube;

vec3 lightPos = { 0,2.5,-5 };
float shininess = 32;
float specularStrength = 1;
Model* model;
Shader* sh;
void ShowCaseScene::Start(GLFWwindow* win)
{
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // seteaza cursorul lockat pe window

	float far = 100;
	float near = 0.1;
	float fov = pi / 4;
	cam = new Camera(glm::perspective(fov, ((float)Window_Width / Window_Height), near, far), glm::vec3(0, 0, -10), win);

	cubeMap = new CubeMap();
	cube = new CubeModel();
	cube->color = vec4(1);

	sh = new Shader(SHADER_PATH "DefaultMesh.shader");
	model = new Model(MODEL_PATH "Backpack/backpack.obj", sh);
}

void ShowCaseScene::Update()
{
}

void GuiRender()
{
	ImGui::SliderFloat3("Light Position", &lightPos.x, -5, 5);
	ImGui::SliderFloat("Shininess", &shininess, 0, 200);
	ImGui::SliderFloat("SpecularStrength", &specularStrength, 0, 1);
}

void ShowCaseScene::Render()
{
	lightPos = mat3(rotate(mat4(1), (float)pi / 160, vec3(0, 1, 0))) * lightPos;
	cubeMap->Draw(*cam);
	cam->UpdateInput();
	cube->viewMat = translate(mat4(1), lightPos);
	cube->Draw(*cam);
	sh->SetUniform3f("u_lightPos", lightPos);
	sh->SetUniform1f("u_shininess", shininess);
	sh->SetUniform1f("u_specularStrength", specularStrength);
	model->Draw(*cam);

	GuiRender();
}

void ShowCaseScene::Terminate()
{
}
