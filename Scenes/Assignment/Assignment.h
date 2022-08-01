#pragma once
#include "../Scene.h"

class Assignment : public Scene
{
public:
	Assignment() :Scene((string)NAMEOF_SHORT_TYPE(Assignment)) {}
	void Start(GLFWwindow*) override;
	void Render() override;
	void UiRender() override;
	void Update() override;
	void Terminate() override;
	void GuiRender();

};