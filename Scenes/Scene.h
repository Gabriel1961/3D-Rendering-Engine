#pragma once
#include <string>
#include <vector>
#include "Common.h"
#include "../Action/Action.h"
using namespace std;
using namespace glm;
using namespace EventSystem;
class Scene
{
	static Scene* activeScene;
public:
	static vector<Scene*>* availableScenes;
	string name;

	Scene(const string& name);
	/// <param name="scene"> if scene is null the current loaded scene will be disposed </param>
	static void SetActiveScene(Scene* scene, GLFWwindow* window);

	static void StartActiveScene(GLFWwindow* window);
	static void UpdateActiveScene();
	static void RenderActiveScene();
	static void TerminateActiveScene();


	virtual void Start(GLFWwindow*) = 0;
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void Terminate() = 0;
};