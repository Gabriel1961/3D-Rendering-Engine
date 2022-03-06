#include "Camera.h"
#include "./Input/Input.h"
Camera* ths;
/// Fix janky mouse movement
Camera::Camera(const glm::mat4& projMat, const glm::vec3& position, GLFWwindow* window) : projMat(projMat), position(position), window(window) {
	// Event based movement of camera
	/*Input::Mouse::MouseMove += [this](double speedX, double speedY) {
		if (abs(speedX) > 1 || abs(Input::Mouse::SpeedY) > 1)
			return;
		this->rotation.x += speedX;
		this->rotation.y += speedY;
		if (this->rotation.y > pi / 2)
			this->rotation.y = pi / 2;
		if (this->rotation.y < -pi / 2)
			this->rotation.y = -pi / 2;
	};*/
}

static struct MPos { double x{}, y{}; };
void Camera::UpdateInput()
{
#pragma region MouseMovement

	// Loop based mouse movement
	float sensitivity = 0.002;
	static MPos prevMousePos{};
	MPos curMousePos{};
	glfwGetCursorPos(window, &curMousePos.x, &curMousePos.y);
	MPos mouseSpeed = { curMousePos.x - prevMousePos.x, curMousePos.y - prevMousePos.y };
	if (abs(mouseSpeed.x) < 200 && abs(mouseSpeed.y) < 200)
	{
		this->rotation.x += mouseSpeed.x* sensitivity;
		this->rotation.y += mouseSpeed.y*-sensitivity;
		if (this->rotation.y > pi / 2)
			this->rotation.y = pi / 2;
		if (this->rotation.y < -pi / 2)
			this->rotation.y = -pi / 2;
	}
	prevMousePos = curMousePos;

#pragma endregion

#pragma region Enable/Disable Cursor

	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		if (isMouseLocked == false)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED), isMouseLocked = true;
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL), isMouseLocked = false;
	}
	
#pragma endregion

#pragma region KeyMovement
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
#pragma endregion

#pragma region CamSpeedChange
	if (glfwGetKey(window, GLFW_KEY_UP))
		camSpeed += 0.001;
	else if (glfwGetKey(window, GLFW_KEY_DOWN))
		camSpeed -= 0.001;
#pragma endregion

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
