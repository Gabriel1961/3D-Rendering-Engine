#pragma once 
#include <3DRenderingEngine.h>
#include "Camera.h"
class CubeModel
{

public:
	glm::vec4 color = glm::vec4(1, 0, 1, 1);
	glm::mat4 modelMat = glm::mat4(1);
	glm::mat4 viewMat = glm::mat4(1);
	VertexArray* va;
	VertexBuffer* vb;
	IndexBuffer* ib;
	Shader* sh;
	CubeModel(std::string shader = SHADER_PATH "DefaultMesh.shader");
	void Draw(const Camera& cam);
};
