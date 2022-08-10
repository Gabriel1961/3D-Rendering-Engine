#pragma once 
#include <3DRenderingEngine.h>
#include "Modules/Camera.h"
#include "Modules/Material/Material.h"
class CubeModel
{

public:
	glm::mat4 modelMat = glm::mat4(1);
	glm::mat4 viewMat = glm::mat4(1);
	Material* mat;
	VertexArray* va;
	VertexBuffer* vb;
	IndexBuffer* ib;
	Shader* sh;
	
	CubeModel(const std::string& shader = SHADER_PATH "PrebuiltCube.shader");
	~CubeModel();
	void Render(const Camera& cam);
};
