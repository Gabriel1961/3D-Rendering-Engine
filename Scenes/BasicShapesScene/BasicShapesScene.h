#pragma once
#include "../Scene.h"
#define ASSETS_PATH "Scenes/BasicShapesScene/Assets/" 
class BasicShapesScene : public Scene
{
public:
	BasicShapesScene() :Scene((string)NAMEOF_SHORT_TYPE(BasicShapesScene)) {}
	void Start(GLFWwindow*) override;
	void BasicRender(const Camera& cam, shared_ptr<Shader> shOverr=0);
	void Render() override;
	void UiRender() override;
	void Update() override;
	void Terminate() override;

};