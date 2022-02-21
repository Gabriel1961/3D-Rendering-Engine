#include "Camera.h"
#include "./Input/Input.h"
Camera* ths;
Camera::Camera(const glm::mat4& projMat, const glm::vec3& position, GLFWwindow* window) : projMat(projMat), position(position), window(window) {

	Input::Mouse::MouseMove += [this](double speedX, double speedY) {
		if (abs(speedX) > 1 || abs(Input::Mouse::SpeedY) > 1)
			return;
		this->rotation.x += speedX;
		this->rotation.y += speedY;
		if (this->rotation.y > pi / 2)
			this->rotation.y = pi / 2;
		if (this->rotation.y < -pi / 2)
			this->rotation.y = -pi / 2;
	};
}

void Camera::UpdateInput()
{
	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		if (isMouseLocked == false)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED), isMouseLocked = true;
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL), isMouseLocked = false;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
		position.y += camSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
		position.y -= camSpeed;
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		position.x -= cos(rotation.x) * camSpeed;
		position.z -= sin(rotation.x) * camSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		position.x += cos(rotation.x) * camSpeed;
		position.z += sin(rotation.x) * camSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		position.y -= sin(rotation.y) * camSpeed;
		position.x -= sin(rotation.x) * camSpeed;
		position.z += cos(rotation.x) * camSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		position.y += sin(rotation.y) * camSpeed;
		position.x += sin(rotation.x) * camSpeed;
		position.z -= cos(rotation.x) * camSpeed;
	}

}

glm::mat4 Camera::GetCamRotMat() const
{
	using namespace glm;
	mat4 camRot(1);
	{
		mat3 camRotX = mat3
		(
			cos(rotation.x), 0, sin(-rotation.x),
			0, 1, 0,
			sin(rotation.x), 0, cos(rotation.x)
		);
		mat3 camRotY = mat3
		(
			1, 0, 0,
			0, cos(rotation.y), sin(-rotation.y),
			0, sin(rotation.y), cos(rotation.y)
		);
		camRot = mat4(camRotY * camRotX);
	}
	return camRot;
}
