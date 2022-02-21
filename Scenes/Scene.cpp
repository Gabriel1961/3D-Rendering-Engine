#include "Scene.h"
Scene* Scene::activeScene = 0;
vector<Scene*>* Scene::availableScenes = 0;

Scene::Scene(const string& name)
	:name(name)
{
	if (availableScenes == 0)
		availableScenes = new vector<Scene*>();
	availableScenes->push_back(this);
}

/// <param name="scene"> if scene is null the current loaded scene will be disposed </param>

void Scene::SetActiveScene(Scene* scene, GLFWwindow* window)
{
	if (activeScene != 0)
		activeScene->Terminate();
	activeScene = scene;

	if (scene == 0)
		return;
}

void Scene::UpdateActiveScene()
{
	if (activeScene == 0)
		return;
	else
		activeScene->Update();
}

void Scene::RenderActiveScene()
{
	if (activeScene == 0)
		return;
	else
		activeScene->Render();
}

void Scene::StartActiveScene(GLFWwindow* window)
{
	if (activeScene == 0)
		return;
	else
		activeScene->Start(window);
}

void Scene::TerminateActiveScene()
{
	if (activeScene == 0)
		return;
	else
		activeScene->Terminate();
}
