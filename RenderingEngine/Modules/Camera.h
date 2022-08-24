#pragma once
#include <Renderer.h>

namespace Physics
{
	struct Ray;
}
class Scene;
class Model;
class TransformGizmo3D;
class Camera
{
	shared_ptr<TransformGizmo3D> transformGizmo = 0;
	
public:
	GLFWwindow* window;
	Camera(bool handleInput, GLFWwindow* window,Scene* s, const glm::vec3& position,const mat4& proj = glm::perspective(pi/2.f,AspectR,.1f,1000.f));

	Camera(const glm::mat4& proj);
	void UpdateInput();
	Scene* s;
	shared_ptr<Model> clickSelectTgt = 0;
	
	void UpdateClickSelectInput(const std::vector<shared_ptr<Model>>& models);
	
	float camSpeed = 0.1;
	bool handleInput;
	bool isMouseLocked = false;
	glm::mat4 projMat;
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 position;
	glm::mat4 GetCamRotMat() const;
	glm::mat4 GetViewMat() const;
	Physics::Ray GetMouseRay();
};