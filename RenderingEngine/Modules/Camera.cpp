#include "Camera.h"
#include "./Input/Input.h"
#include "Modules/Physics/BoundingBox.h"
#include "Modules/Mesh.h"
#include "Modules/Gizmos/Gizmo3D.h"
#include "../Scenes/Scene.h"
Camera::Camera(bool handleInput, GLFWwindow* window, Scene* s, const glm::vec3& position, const mat4& proj) : position(position), window(window),handleInput(handleInput),projMat(proj),s(s)
{
	shared_ptr<TransformGizmo3D> giz= make_shared<TransformGizmo3D>();;
	currentMovableGizmo = giz;
	currentMovableGizmo->SetEnabled(false);
	if (s)
		s->gizmos.push_back(giz);
}

static struct MPos { double x{}, y{}; };
Camera::Camera(const glm::mat4& proj) : projMat(proj),handleInput(0),s(0),window(),position(0)
{
}
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

void Camera::UpdateClickSelectInput(const std::vector<shared_ptr<Model>>& models)
{
	using namespace Physics;
	static vec2 prevMousePos;
	static int prevLeftPressed = glfwGetMouseButton(window,0);
	static int selctedAxis = -1;
	if (glfwGetMouseButton(window, 0) == GLFW_PRESS)
	{
		for (auto c : models)
			c->CalculateBoundingBox();

		Ray r = GetMouseRay();
		float closestDist = INFINITY;
		Rayhit hit;
		shared_ptr<Model> md = 0;

		Rayhit rh{};
		for (auto c : models)
			if (c->boundingBox.Intersect(r, rh))
			{
				if (rh.dist < closestDist) {
					closestDist = rh.dist;
					hit = rh;
					md = c;
				}
			}

		vec2 curMousePos = { Input::Mouse::X,Input::Mouse::Y };
		if (prevLeftPressed == false)
		{ // first press

			if (md == 0)
			{
				currentMovableGizmo->SetEnabled(false);
				return;
			}
			
			selctedAxis = currentMovableGizmo->GetIntersectedAxis(r);
			clickSelectTgt = md;
			currentMovableGizmo->SetEnabled(false);
			currentMovableGizmo->SetParentModel(weak_ptr<Model>());
		}
		else if (clickSelectTgt)
		{ // held down 
			float sensitivity = 6;
			vec2 delta = curMousePos - prevMousePos;
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			currentMovableGizmo->SetParentModel(clickSelectTgt);
			currentMovableGizmo->SetEnabled(true);
			currentMovableGizmo->UpdateModelTransform(VectorToWorld(vec3(delta,0)),projMat * GetViewMat(), selctedAxis);
		}
		else
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
		}
		prevMousePos = curMousePos;
		prevLeftPressed = true;
	}
	else prevLeftPressed = false;
}
vec3 Camera::VectorToWorld(vec3 pt)
{
	mat3 icamRot = rotate(rotate(mat4(1), -rotation.x, { 0,1,0 }), rotation.y, { 1,0,0 });
	return icamRot * pt;
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
	v = viewToWorld * vec4(v,1);
	return Physics::Ray(position, v);
}

