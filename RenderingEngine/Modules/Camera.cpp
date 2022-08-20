#include "Camera.h"
#include "./Input/Input.h"
#include "Modules/Physics/BoundingBox.h"

Camera::Camera(float fov, float ar, float znear, float zfar, const glm::vec3& position, bool handleInput, GLFWwindow* window) : position(position), window(window),handleInput(handleInput),znear(znear),zfar(zfar),ar(ar),fov(fov),projMat(glm::perspective(fov,ar,znear,zfar))
{

}

static struct MPos { double x{}, y{}; };
void Camera::UpdateInput()
{
	if (handleInput == false)
		return;
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
	
	static MPos prevMousePos{};
	MPos curMousePos{};
	glfwGetCursorPos(window, &curMousePos.x, &curMousePos.y);
	if (isMouseLocked)
	{

		float sensitivity = 0.002;
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
	}
	prevMousePos = curMousePos;

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

glm::mat4 Camera::GetViewMat() const
{
	return translate(GetCamRotMat(), {-position});
}

Physics::Ray Camera::GetMouseRay()
{
	double x, y;
	int height,width;
	glfwGetWindowSize(window, &width, &height);
	glfwGetCursorPos(window, &x, &y);
	
	vec3 v((2*x-width)/height, (-2.*y + height)/height,-1);
	v = normalize(v);
	mat4 viewToWorld = inverse(mat3(GetViewMat()));
	std::cout << v.x << " " << v.y << " " << v.z << "\n";
	v = viewToWorld * vec4(v,1);
	return Physics::Ray(position, v);
}

void Camera::DrawSelectedOutline()
{
}
