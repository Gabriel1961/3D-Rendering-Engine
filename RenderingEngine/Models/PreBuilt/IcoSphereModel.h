#pragma once
#include "Modules/Mesh.h"
#include <3DRenderingEngine.h>
static const float X = .525731112119133606f;
static const float Z = .850650808352039932f;
static const float N = 0.f;

class IcoSphereModel : public Mesh
{
public:
	glm::vec4 color = {1,1,1,1};
	std::vector<uint>* triangles;
	std::vector<glm::vec3>* vertexPositions;
	IcoSphereModel(int subdivisionLevel, shared_ptr<Shader>_shader );
	void Render(const Camera& camera) override;
};
