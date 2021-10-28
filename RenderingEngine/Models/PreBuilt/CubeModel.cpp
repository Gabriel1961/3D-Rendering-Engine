#include "CubeModel.h"
static 	float vertexes[]
{/* pos      norm   uv */
	-1,-1,-1, 0,0,0, 0,0,
	1,-1,-1,  0,0,0, 0,0,
	1,-1,1,   0,0,0, 0,0,
	-1,-1,1,  0,0,0, 0,0,
	-1,1,-1,  0,0,0, 0,0,
	1,1,-1,	  0,0,0, 0,0,
	1,1,1,	  0,0,0, 0,0,
	-1,1,1,	  0,0,0, 0,0,
};
static uint indexes[]
{
	0,4,5,
	0,5,1,
	1,5,6,
	1,6,2,
	2,6,7,
	2,7,3,
	3,7,4,
	3,4,0,
	4,7,6,
	4,6,5,
	0,2,3,
	0,1,2,
};
CubeModel::CubeModel(std::string shader)
{
	vb = new VertexBuffer(sizeof(vertexes), vertexes);
	ib = new IndexBuffer(36, indexes, GL_UNSIGNED_INT);
	va = new VertexArray();
	VertexBufferLayout vbl;
	vbl.Push<float>(3);
	vbl.Push<float>(3);
	vbl.Push<float>(2);
	va->AddLayout(*vb, vbl);
	sh = new Shader(shader);
}

CubeModel::~CubeModel()
{
	delete va;
	delete vb;
	delete ib;
}

void CubeModel::Draw(const Camera& cam)
{
	sh->SetUniformMat4f("u_MVP", cam.GetMVP(modelMat, viewMat));
	sh->SetUniform3f("u_camPos", cam.position);
	sh->SetUniform4f("u_color", color);
	Renderer::Draw(*va, *ib, *sh);
}
