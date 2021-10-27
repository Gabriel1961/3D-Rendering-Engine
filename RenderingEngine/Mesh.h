#pragma once
#include <Renderer.h>
#include <Common.h>
#include <Transform.h>
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};
class Mesh
{
public:

	VertexBuffer* vb = 0;
	IndexBuffer* ib = 0;
	VertexArray* va = 0;
	Shader* sh = 0;

	std::vector <Vertex> vertexes;
	std::vector <uint> indexes;
	std::vector <Texture*> textures;

	TransformGroup* transformGroup = new TransformGroup();

	Mesh(const std::vector<Vertex>& vertexes, const std::vector<uint>& indexes, const std::vector<Texture*>& textures);
	void Draw();
	~Mesh();

protected:
	void SetupMesh();
};
