#include "3DRenderingEngine.h"
#include "../Scenes/Scene.h"
using namespace std;
using namespace glm;
GLFWwindow* window;
void RenderingEngine::Start(GLFWwindow* win)
{
	window = win;
	gc(glEnable(GL_DEPTH_TEST));
	gc(glDepthFunc(GL_LESS));
	Input::Init(win);
	Scene::SetActiveScene("BasicShapesScene", win);
	Scene::StartActiveScene(window);
}

void RenderingEngine::Render()
{
	Scene::RenderActiveScene();
}

void RenderingEngine::Update()
{
	Scene::UpdateActiveScene();
}

void RenderingEngine::Terminate()
{
	Scene::TerminateActiveScene();
}
