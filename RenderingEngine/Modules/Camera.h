#pragma once
#include <Renderer.h>
class Camera
{
public:
	GLFWwindow* window;
	Camera(const glm::mat4& projMat, const glm::vec3& position, GLFWwindow* window);
	void UpdateInput();
	float camSpeed = 0.1;
	bool isMouseLocked = false;
	glm::mat4 projMat;
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 position;
	glm::mat4 GetCamRotMat() const;
};
