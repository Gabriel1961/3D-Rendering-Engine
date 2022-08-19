#include "IcoSphereModel.h"
#include <math.h>
using namespace std;


vector<uint>& SubDivide(int subdivisionLevel, IcoSphereModel* ico)
{
	if (subdivisionLevel < 0)
		return (*ico->triangles);
	int totalSize = 0;
	int lastSize = (*ico->triangles).size() * pow(4, subdivisionLevel);

	for (int i = 0,t = (*ico->triangles).size(); i < subdivisionLevel; i++)
	{
		totalSize += t;
		t *= 4;
	}

	for(int i =0;i<totalSize;i+=3)
	{
		uint a = (*ico->triangles)[i], b = (*ico->triangles)[i+1], c = (*ico->triangles)[i+2];
		glm::vec3 pa = (*ico->vertexPositions)[a],
			pb = (*ico->vertexPositions)[b],
			pc = (*ico->vertexPositions)[c];
		glm::vec3 ma = (pa + pb) / 2.0f,mb = (pb+pc)/2.0f, mc = (pa+pc)/2.0f;

		int ia = (*ico->vertexPositions).size(), ib = ia + 1, ic = ib + 1;
		// Pushback new vertexes
		(*ico->vertexPositions).push_back(normalize(ma));
		(*ico->vertexPositions).push_back(normalize(mb));
		(*ico->vertexPositions).push_back(normalize(mc));

		(*ico->triangles).push_back(a);
		(*ico->triangles).push_back(ia);
		(*ico->triangles).push_back(ic);

		(*ico->triangles).push_back(ic);
		(*ico->triangles).push_back(ia);
		(*ico->triangles).push_back(ib);

		(*ico->triangles).push_back(ia);
		(*ico->triangles).push_back(b);
		(*ico->triangles).push_back(ib);

		(*ico->triangles).push_back(ic);
		(*ico->triangles).push_back(ib);
		(*ico->triangles).push_back(c);
	}
	(*ico->triangles).erase((*ico->triangles).begin(), (*ico->triangles).end() - lastSize);
	return (*ico->triangles);
}

vector<Vertex> GetVertices(int subdivisionLevel, IcoSphereModel* ico)
{
	ico->triangles = new vector<uint>(
		{
		  0,4,1,0,9,4,9,5,4,4,5,8,4,8,1,
		  8,10,1,8,3,10,5,3,8,5,2,3,2,7,3,
		  7,10,3,7,6,10,7,11,6,11,0,6,0,1,6,
		  6,1,10,9,0,11,9,11,2,9,2,5,7,2,11
		});
	ico->vertexPositions = new vector<glm::vec3>(
		{
	  {-X,N,Z}, {X,N,Z} ,{-X,N,-Z},{ X,N,-Z  },
	  {N,Z,X }, {N,Z,-X},{ N,-Z,X },{ N,-Z,-X },
	  {Z,X,N }, {-Z,X,N},{ Z,-X,N },{ -Z,-X, N}
		});

	SubDivide(subdivisionLevel,ico);
	vector<Vertex> vertices((*ico->vertexPositions).size());
	for (int i = 0; i < (*ico->vertexPositions).size(); i++) {
		auto& pos = (*ico->vertexPositions)[i];
		vec2 uv = { 0.5f + atan2f(pos.x,pos.z)/(2.0f*pi),.5f+asin(pos.y)/pi};
		vertices[i] = { (*ico->vertexPositions)[i],(*ico->vertexPositions)[i], uv};
	}
	return vertices;
}


IcoSphereModel::IcoSphereModel(int subdivisionLevel, shared_ptr<Shader> _shader)
{
	vertexes = GetVertices(subdivisionLevel, this);
	indexes = std::move( * triangles);
	delete triangles;
	sh = _shader;
	SetupMesh();
}

void IcoSphereModel::Render(const Camera& camera)
{
	sh->SetUniform4f("u_color", color);
	Mesh::Render(camera);
}
 