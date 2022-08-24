#include "CubeModel.h"
#include "Modules/Mesh.h"
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
static std::vector<uint> indexes
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
CubeModel::CubeModel(std::shared_ptr<Material> mat,std::shared_ptr<Shader>shader)
	:Model()
{

	using namespace std;
	vector<Vertex> verts;
	for (int i = 0; i < 8; i++) {
		verts.push_back({});
		memcpy(&*verts.rbegin(), vertexes + i * 8, sizeof(Vertex));
	}
	Mesh cubeMesh(verts, indexes, {}, mat, shader);

	meshes.push_back(cubeMesh);
	m = &meshes[0];
	m->sh = shader;
}