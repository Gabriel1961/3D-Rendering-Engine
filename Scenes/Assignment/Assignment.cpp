#include "Assignment.h"
Assignment instance;
static Camera* cam;
static CubeModel* cube;
static CubeModel* cube1;
static Texture* tex;
static Texture* tex1;
void Assignment::Start(GLFWwindow* win)
{
	float far = 100;
	float near = 0.1;
	float fov = pi / 4;
	cam = new Camera(glm::perspective(fov, ((float)Window_Width / Window_Height), near, far), glm::vec3(0, 0, 10), win);
	
	cube = new CubeModel();
	cube1 = new CubeModel();
	cube1->modelMat = glm::translate(mat4(1), { 3,0,0 });
	tex = new Texture("Scenes/BasicShapesScene/Assets/earth.jpg");
	tex1 = new Texture("Scenes/BasicShapesScene/Assets/3242sd.png");
}

void Assignment::Render()
{
	cam->UpdateInput();
	tex->Bind(0);
	cube->Render(*cam);
	tex1->Bind(0);
	cube1->Render(*cam);

}

void Assignment::UiRender()
{
}

void Assignment::Update()
{
}

void Assignment::Terminate()
{
}

void Assignment::GuiRender()
{
}
