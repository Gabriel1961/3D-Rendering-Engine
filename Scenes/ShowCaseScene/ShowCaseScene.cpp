#include "./ShowCaseScene.h"
using namespace std;
using namespace glm;
ShowCaseScene showCaseScene;

void ShowCaseScene::Start(GLFWwindow* win)
{
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // seteaza cursorul lockat pe window

	float far = 100;
	float near = 0.1;
	float fov = pi / 4;
	cam = new Camera(fov, ((float)Window_Width / Window_Height), near, far, glm::vec3(0, 0, 10),1, win);

	cubeMap = new CubeMap();
	sh = make_shared<Shader>(SHADER_PATH "DefaultMesh.shader");
	cube = make_shared<CubeModel>(make_shared<Material>(), sh);

	model = new Model(MODEL_PATH "Backpack/backpack.obj", sh);
	for (auto& m : model->meshes)
		m.modelMat = translate(mat4(1),{0,0,2});
}

void ShowCaseScene::Update()
{
}

void ShowCaseScene::GuiRender()
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
	cube->m->viewMat = translate(mat4(1), lightPos);
	cube->Render(*cam);
	sh->SetUniform3f("u_lightPos", lightPos);
	sh->SetUniform1f("u_shininess", shininess);
	sh->SetUniform1f("u_specularStrength", specularStrength);
	model->Render(*cam);

	GuiRender();
}

void ShowCaseScene::UiRender()
{

}

void ShowCaseScene::Terminate()
{
}
