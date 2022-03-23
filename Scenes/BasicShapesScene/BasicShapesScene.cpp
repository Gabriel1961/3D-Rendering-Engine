#include "BasicShapesScene.h"
BasicShapesScene basicShapeScene;
static int icoLod = 0;
static Shader* icoShader = 0;
static LitShader* lshader = 0;
void BasicShapesScene::Start(GLFWwindow* win)
{
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // seteaza cursorul lockat pe window

	float far = 100;
	float near = 0.1;
	float fov = pi / 4;
	cam = new Camera(glm::perspective(fov, ((float)Window_Width / Window_Height), near, far), glm::vec3(0, 0, 10), win);
	icoShader = new Shader(SHADER_PATH "Viewport.shader");
	ico = new IcoSphereModel(3,icoShader);
	cube = new CubeModel();
	cube->viewMat = scale(translate(mat4(1),{ 0, -2, 0 }), { 10,.1,10 });
	cube->color = Color::FromRgb(52, 140, 49).rgba;


	lshader = new LitShader();
	lshader->lights.push_back(Light(LightType::PointLight,{0,4,0}));
	lshader->ApplyLights();

	cube->sh = lshader;
}

void BasicShapesScene::Render()
{
	ico->Render(*cam);
	cube->Render(*cam);
	lshader->RenderLightGizmos(*cam);

}

void BasicShapesScene::Update()
{
	cam->UpdateInput(); 
}

void BasicShapesScene::Terminate()
{
	delete ico;
	delete cube;
}
