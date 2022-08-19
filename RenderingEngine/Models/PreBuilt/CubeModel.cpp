#include "CubeModel.h"
static float vertexes[]
{/* pos      norm   uv */
	-1,-1,-1, 0,-1,0, 0,0, // 0 
	1,-1,-1,  1,0,0, 1,0, // 1
	1,-1,1,   0,0,1, 0,0, // 2
	-1,-1,1,  -1,0,0, 1,0, // 3
	-1,1,-1,  0,1,0, 0,1, // 4
	1,1,-1,	  0,0,-1, 1,1, // 5
	1,1,1,	  0,0,0, 0,1, // 6
	-1,1,1,	  0,0,0, 1,1, // 7
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
CubeModel::CubeModel(std::shared_ptr<Material> mat,const std::string& shader)
	:mat(mat)
{
	vb = make_shared<VertexBuffer>(sizeof(vertexes), vertexes);
	ib = make_shared<IndexBuffer>(36, indexes, GL_UNSIGNED_INT);
	va = make_shared<VertexArray>();
	VertexBufferLayout vbl;
	vbl.Push<float>(3);
	vbl.Push<float>(3);
	vbl.Push<float>(2);
	va->AddLayout(*vb, vbl);
	if (shader != "")
		sh = make_shared<Shader>(shader);
}


void CubeModel::Render(const Camera& camera)
{
	viewMat = camera.GetViewMat();
	mat->Bind(sh);
	sh->SetUniformMat4f("u_model", modelMat);
	sh->SetUniformMat4f("u_view", viewMat);
	sh->SetUniformMat4f("u_proj", camera.projMat);
	sh->SetUniformMat3f("u_normalMat", transpose(inverse(mat3(modelMat))));
	vec3 v = transpose(inverse(mat3(modelMat))) * vec3(0, 1, 0);
	sh->SetUniform3f("u_viewPos", camera.position);
	Renderer::Draw(*va, *ib, *sh);
}
