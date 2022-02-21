#pragma once
#include "../Scene.h"
#include "nameof.hpp"
class ShowCaseScene : public Scene
{
public:
	ShowCaseScene() :Scene((string)NAMEOF_TYPE(ShowCaseScene)) {}
	void Start(GLFWwindow*) override;
	void Render() override;
	void Update() override;
	void Terminate() override ;
};