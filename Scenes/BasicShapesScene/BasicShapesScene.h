#pragma once
#include "../Scene.h"
class BasicShapesScene : public Scene
{
public:
	BasicShapesScene() :Scene((string)NAMEOF_SHORT_TYPE(BasicShapesScene)) {}
	void Start(GLFWwindow*) override;
	void Render() override;
	void Update() override;
	void Terminate() override;

	Camera* cam{};
	IcoSphereModel* ico{};
	CubeModel* cube{};
};