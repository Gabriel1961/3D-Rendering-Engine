#include "Scene.h"
Scene* Scene::activeScene = 0;
vector<Scene*>* Scene::availableScenes = 0;

void Scene::SetWindowPtr(GLFWwindow* ptr)
{
	window = ptr;
	mainCamera->window = ptr;
}

shared_ptr<Camera> CreateDefaultCamera(Scene* s)
{
	shared_ptr<Camera> mainCamera;
	// Create the default camera
	float far = 100;
	float near = 0.1;
	float fov = pi / 4;
	mainCamera = make_shared<Camera>(1, (GLFWwindow*)0,s, glm::vec3(0, 0, 10));
	return mainCamera;
}

void Scene::DrawModels(const Camera& cam, shared_ptr<Shader> shOverr)
{
	for (int i=0;i<models.size();i++)
		models[i]->Render(cam,shOverr);
}

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

void Scene::SetActiveScene(const std::string& sceneName, GLFWwindow* window)
{
	for (auto& s : *availableScenes)
	{
		if (s->name == sceneName)
		{
			SetActiveScene(s, window);
			return;
		}
	}
	cout << "Scene " << sceneName << "not found\n";
	assert(false);
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
	{
		activeScene->Render();

	}
}

void Scene::UiRenderActiveScene()
{
	if (activeScene == 0)
		return;
	else
	{
		activeScene->UiRender();
	}
}

void Scene::StartActiveScene(GLFWwindow* window)
{
	if (activeScene == 0)
		return;
	else
	{
		if (activeScene->mainCamera == 0)
			activeScene->mainCamera = CreateDefaultCamera(activeScene);
		activeScene->SetWindowPtr(window);
		activeScene->Start(window);
	}
}

void Scene::TerminateActiveScene()
{
	if (activeScene == 0)
		return;
	else
	{
		activeScene->Terminate();
	}
}
