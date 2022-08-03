#include "Camera.h"
#include "./Input/Input.h"
Camera::Camera(const glm::mat4& projMat, const glm::vec3& position, GLFWwindow* window) : projMat(projMat), position(position), window(window) {
#pragma region Enable/Disable Cursor


#pragma endregion
}

static struct MPos { double x{}, y{}; };
void Camera::UpdateInput()
{

#pragma region Cursor
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		isMouseLocked = true;
		
		glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR_DISABLED); // set cursor to locked 
	}
	else {
		isMouseLocked = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
#pragma endregion

	
#pragma region MouseMovement
	
	if (isMouseLocked)
	{

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
	}

#pragma endregion


#pragma region KeyMovement
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
		position.y += camSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
		position.y -= camSpeed;
	vec3 delta = {0,0,0};
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		delta.x -= camSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		delta.x += camSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		delta.z -= camSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		delta.z += camSpeed;
	}
	position = inverse(GetCamRotMat()) * translate(mat4(1), delta) * GetCamRotMat() * vec4(position, 1);

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
	mat4 camRot = rotate(rotate(mat4(1), -rotation.y, { 1,0,0 }), rotation.x, { 0,1,0 });
	return camRot;
}
