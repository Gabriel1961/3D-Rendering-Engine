#pragma once
#include "../Scene.h"
#include "nameof.hpp"
class BasicShapesScene : public Scene
{
public:
	BasicShapesScene() :Scene((string)NAMEOF_TYPE(BasicShapesScene)) {}
	void Start(GLFWwindow*) override;
	void Render() override;
	void Update() override;
	void Terminate() override;
};