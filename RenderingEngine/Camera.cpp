#include "Camera.h"

Camera::Camera(const glm::mat4& projMat, const glm::vec3& position) : projMat(projMat), position(position) {}

glm::mat4 Camera::GetMVP(const glm::mat4& model, const glm::mat4& view) const
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
	return projMat * camRot * view * model;
}
