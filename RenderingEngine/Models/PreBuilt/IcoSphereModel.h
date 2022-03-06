#pragma once
#include "./Mesh.h"
#include <3DRenderingEngine.h>
static const float X = .525731112119133606f;
static const float Z = .850650808352039932f;
static const float N = 0.f;

class IcoSphereModel : public Mesh
{
public:
	std::vector<uint>* triangles;
	std::vector<glm::vec3>* vertexPositions;
	IcoSphereModel(int subdivisionLevel,Shader*_shader );
};
