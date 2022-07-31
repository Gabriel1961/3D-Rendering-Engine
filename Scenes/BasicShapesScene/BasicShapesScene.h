#pragma once
#include "../Scene.h"
#define ASSETS_PATH "Scenes/BasicShapesScene/Assets/" 
class BasicShapesScene : public Scene
{
public:
	BasicShapesScene() :Scene((string)NAMEOF_SHORT_TYPE(BasicShapesScene)) {}
	void Start(GLFWwindow*) override;
	void BasicRender();
	void Render() override;
	void UiRender() override;
	void Update() override;
	void Terminate() override;

	IcoSphereModel* ico{};
	CubeModel* cube{};
};