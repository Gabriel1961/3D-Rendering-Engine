#include "CubeModel.h"
static float vertexes[]
{/* pos      norm   uv */
	-1,-1,-1, 0,-1,0, 0,0, // 0 
	1,-1,-1,  1,0,0, 0,0, // 1
	1,-1,1,   0,0,1, 0,0, // 2
	-1,-1,1,  -1,0,0, 0,0, // 3
	-1,1,-1,  0,1,0, 0,0, // 4
	1,1,-1,	  0,0,-1, 0,0, // 5
	1,1,1,	  0,0,0, 0,0, // 6
	-1,1,1,	  0,0,0, 0,0, // 7
};
static uint indexes[]
{
	0,4,5,
	1,0,5,
	5,6,1,
	6,2,1,
	6,7,2,
	7,3,2,
	7,4,3,
	4,0,3,
	7,6,4,
	6,5,4,
	2,3,0,
	1,2,0,
};
CubeModel::CubeModel(const std::string& shader)
{
		
	vb = new VertexBuffer(sizeof(vertexes), vertexes);
	ib = new IndexBuffer(36, indexes, GL_UNSIGNED_INT);
	va = new VertexArray();
	VertexBufferLayout vbl;
	vbl.Push<float>(3);
	vbl.Push<float>(3);
	vbl.Push<float>(2);
	va->AddLayout(*vb, vbl);
	if (shader != "")
		sh = new Shader(shader);
}

CubeModel::~CubeModel()
{
	delete va;
	delete vb;
	delete ib;
	delete sh;
}

void CubeModel::Render(const Camera& camera)
{
	sh->SetUniform1i("useTex", 0);
	sh->SetUniformMat4f("u_model", modelMat);
	sh->SetUniformMat4f("u_view", viewMat);
	sh->SetUniformMat4f("u_projection", camera.projMat);
	sh->SetUniformMat4f("u_camMat", camera.GetCamRotMat());
	sh->SetUniformMat3f("u_normalMVMat", transpose(inverse(mat3(modelMat))));
	sh->SetUniform3f("u_camPos", camera.position);
	sh->SetUniform4f("u_color", color);
	Renderer::Draw(*va, *ib, *sh);
}
