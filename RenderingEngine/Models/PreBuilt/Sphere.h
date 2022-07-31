#pragma once
#include "Modules/Mesh.h"
#include <3DRenderingEngine.h>
#include <vector>

class Sphere : public Mesh
{
private:
	std::vector<Vertex> CreateVertexes(ivec2 size);
	std::vector<uint> CreateIndexes(ivec2 size);
public:
	glm::vec4 color = { 1,1,1,1 };
	Sphere(ivec2 size);
	void Render(const Camera& camera) override;
};

