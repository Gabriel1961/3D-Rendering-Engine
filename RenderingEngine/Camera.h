#pragma once
#include <Renderer.h>
class Camera
{
public:
	Camera(const glm::mat4& projMat, const glm::vec3& position);
	glm::mat4 projMat;
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 position;
	glm::mat4 GetCamMat() const;
};
