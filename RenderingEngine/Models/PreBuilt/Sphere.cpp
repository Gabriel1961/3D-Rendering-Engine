#include "Sphere.h"
#include <math.h>
using namespace std;
std::vector<Vertex> SphereModel::CreateVertexes(ivec2 size)
{
	std::vector<Vertex> verts;
	float thetaStep = 2 * pi / size.x,phiStep = pi/size.y;
	float theta = 0, phi = pi/2-phiStep;
	for (int i = 0; i < size.y-1;i++) {
		theta = 0;

		for (int j = 0; j <= size.x; j++) {

			float cp = cos(phi);
			float sp = sin(phi);
			float ct = cos(theta);
			float st = sin(theta);

			vec2 uv = { theta / (2 * pi),.5 + phi / pi };
			vec3 pos = vec3(cp * st, sp, cp * ct);
			verts.push_back({ pos,pos,uv});
			theta += thetaStep;
		}
		phi -= phiStep;
	}

	return verts;
}

std::vector<uint> SphereModel::CreateIndexes(ivec2 size, vector<Vertex>& vertexes)
{
	std::vector<uint> indexes;
	uint upIdx = vertexes.size(),downIdx = upIdx +1;
	vertexes.push_back({ {0,1,0},{0,1,0} });
	vertexes.push_back({ {0,-1,0},{0,-1,0} });


	int m = size.x +1; // + 1 for the duplicated vertexes
	for (int i = 0; i < size.y-2; i++) {
		for (int j = 0; j < size.x; j++) {
			indexes.push_back(i*m+j+1);
			indexes.push_back(i*m+j);
			indexes.push_back((i + 1) * m + j+1);

			indexes.push_back((i + 1) * m + j+1);
			indexes.push_back(i * m + j);
			indexes.push_back((i + 1) * m + j);
		}
	}

	for (int i = 0; i < size.x; i++) {
		indexes.push_back(upIdx);
		indexes.push_back(i);
		indexes.push_back(i+1);
	}
	for (int i = 0; i < size.x; i++) {
		indexes.push_back(downIdx);
		indexes.push_back((size.y-2) * m +i + 1);
		indexes.push_back((size.y-2)*m + i);
	}
	return indexes;
}

SphereModel::SphereModel(ivec2 size, shared_ptr<Material> mat,shared_ptr<Shader> sh)
{
	auto vertexes = CreateVertexes(size);
	auto indexes = CreateIndexes(size,vertexes);
	meshes.push_back(Mesh(vertexes, indexes, {}, mat, sh));
	m = &meshes[0];
}

