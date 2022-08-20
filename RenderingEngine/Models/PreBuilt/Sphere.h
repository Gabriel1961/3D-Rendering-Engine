#pragma once
#include "Modules/Mesh.h"
#include <3DRenderingEngine.h>
#include <vector>
struct Vertex;
class SphereModel : public Model
{
private:
	static std::vector<Vertex> CreateVertexes(ivec2 size);
	static std::vector<uint> CreateIndexes(ivec2 size, std::vector<Vertex>& vertexes);
public:
	SphereModel(ivec2 size,shared_ptr<Material> mat,shared_ptr<Shader> sh);
	Mesh* m;
};

