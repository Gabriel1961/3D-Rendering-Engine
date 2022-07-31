#pragma once
#include "../Scene.h"

class ShowCaseScene : public Scene
{
public:
	ShowCaseScene() :Scene((string)NAMEOF_SHORT_TYPE(ShowCaseScene)) {}
	void Start(GLFWwindow*) override;
	void Render() override;
	void UiRender() override;
	void Update() override;
	void Terminate() override ;
	void GuiRender();
	Camera* cam;
	CubeMap* cubeMap;
	CubeModel* cube;

	vec3 lightPos = { 0,2.5,-5 };
	float shininess = 32;
	float specularStrength = 1;
	Model* model;
	Shader* sh;
};