#pragma once 
#include <3DRenderingEngine.h>
#include "Modules/Camera.h"
#include "Modules/Material/Material.h"
class CubeModel
{

public:
	glm::mat4 modelMat = glm::mat4(1);
	glm::mat4 viewMat = glm::mat4(1);
	std::shared_ptr<Material> mat;
	shared_ptr<VertexArray> va;
	shared_ptr<VertexBuffer> vb;
	shared_ptr<IndexBuffer> ib;
	shared_ptr<Shader> sh;
	
	CubeModel(std::shared_ptr<Material> mat = std::make_shared<Material>(vec4{ 1, 1, 1,1 }), const std::string& shader = SHADER_PATH "PrebuiltCube.shader");
	void Render(const Camera& cam);
};
