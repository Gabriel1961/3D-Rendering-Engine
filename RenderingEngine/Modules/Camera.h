#pragma once
#include <Renderer.h>

namespace Physics
{
	struct Ray;
}
class Model;
class Camera
{
public:
	const float znear,zfar,fov,ar;
	GLFWwindow* window;
	Camera(float fov, float ar, float znear, float zfar, const glm::vec3& position,bool handleInput, GLFWwindow* window);
	void UpdateInput();

	float camSpeed = 0.1;
	bool handleInput;
	bool isMouseLocked = false;
	glm::mat4 projMat;
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 position;
	glm::mat4 GetCamRotMat() const;
	glm::mat4 GetViewMat() const;
	Physics::Ray GetMouseRay();
	Model* selectedObject = 0;
	void DrawSelectedOutline();
};