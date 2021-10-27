#include "Mesh.h"
#include <Common.h>
#include <Renderer.h>
using namespace std;
using namespace glm;
Mesh::Mesh(const std::vector<Vertex>& vertexes, const std::vector<uint>& indexes, const std::vector<Texture*>& textures)
	: vertexes(vertexes), indexes(indexes), textures(textures)
{
}

void Mesh::Draw()
{
}

Mesh::~Mesh()
{
	delete vb;
	delete ib;
	delete va;
}

void Mesh::SetupMesh()
{
	vb = new VertexBuffer(sizeof(Vertex) * vertexes.size(), &vertexes[0]);
	ib = new IndexBuffer(indexes.size(), &indexes[0], GL_UNSIGNED_INT, GL_STATIC_DRAW);
	VertexBufferLayout vbl;
	vbl.Push<float>(3);
	vbl.Push<float>(3);
	vbl.Push<float>(2);
	va = new VertexArray();
	va->AddLayout(*vb, vbl);
	va->Bind();
}
