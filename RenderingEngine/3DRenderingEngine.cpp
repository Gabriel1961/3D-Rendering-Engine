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

void RenderingEngine::UiRender()
{
	Scene::UiRenderActiveScene();
}

void RenderingEngine::Update()
{
	CheckRecompileAllShaders();
	Scene::UpdateActiveScene();
}

void RenderingEngine::Terminate()
{
	Scene::TerminateActiveScene();
}

void RenderingEngine::CheckRecompileAllShaders()
{
	static uint lastState = GLFW_RELEASE;
	uint state = glfwGetKey(window, GLFW_KEY_R);
	if(state == GLFW_PRESS && lastState != GLFW_PRESS)
	{
		cout << "Recompiling Shaders...\n";
		for (auto& sh : Shader::shaderList) {
			if (sh.second->Recompile() == false) {
				cout << "[Failed Compilation]: " << sh.second->GetFilePath() << "\a\n";
			}
		}
		cout << "Recompiling done.\n";
	}
	lastState = state;
}
