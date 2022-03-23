#include "Mesh.h"
#include <Common.h>
#include <Renderer.h>
#include <Logger.h>
using namespace std;
using namespace glm;
using std::move;
//#define MESH_DEBUG
#ifdef MESH_DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif // DEBUG

#define SAMPLER_DIFFUSE_NAME string("texture_diffuse")
#define SAMPLER_SPECULAR_NAME string("texture_specular")
///  Mesh  ///

Mesh::Mesh(const std::vector<Vertex>& vertexes, const std::vector<uint>& indexes, const std::vector<Texture>& textures,Shader* shader)
	: vertexes(vertexes), indexes(indexes), textures(textures),sh(shader)
{
	SetupMesh();
}

vec3 CalculateNormalAtIndex(std::vector<Vertex>& verts, int a, int b, int c)
{
	vec3 ac = verts[c].position - verts[a].position, ab = verts[b].position - verts[a].position;
	return cross(ac, ab);
}


void Mesh::Render(const Camera& camera)
{
	uint diffuseStartIndex = 1, specularStartIndex = 1, slot = 0;
	sh->Bind();
	for (int i = 0; i < textures.size(); i++)
	{
		if (textures[i].type == SAMPLER_DIFFUSE_NAME)
		{
			textures[i].Bind(slot);
			sh->SetUniform1i(SAMPLER_DIFFUSE_NAME + to_string(diffuseStartIndex), slot);
			diffuseStartIndex++;
			slot++;
		}
		else if (textures[i].type == SAMPLER_SPECULAR_NAME)
		{
			//textures[i].Bind(slot);
			//sh->SetUniform1i(SAMPLER_SPECULAR_NAME + to_string(specularStartIndex), //slot);
			//specularStartIndex++;
			//slot++;
		}
	}
	sh->SetUniformMat4f("u_model", modelMat);
	sh->SetUniformMat4f("u_view", viewMat);
	sh->SetUniformMat4f("u_projection", camera.projMat);
	sh->SetUniformMat4f("u_camMat", camera.GetCamRotMat());
	sh->SetUniformMat3f("u_normalMVMat", transpose(inverse(mat3(viewMat*modelMat))));
	sh->SetUniform3f("u_camPos", camera.position);
	Renderer::Draw(*va, *ib, *sh);
}

Mesh::Mesh(Mesh&& o)noexcept
{
	vb = o.vb;
	va = o.va;
	ib = o.ib;
	sh = o.sh;

	vertexes = move(o.vertexes);
	indexes = move(o.indexes);
	textures = move(o.textures);
	
	modelMat = o.modelMat;
	viewMat = o.viewMat;

	o.va = 0;
	o.vb = 0;
	o.ib = 0;
	o.sh = 0;
}

Mesh::~Mesh()
{
	DBG(print("Deleted mesh"));
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

///  Model  ///

Model::Model(const char* path,Shader* shader)
{
	print("Loading Started");
	LoadModel(path,shader);
	print("Loading Complete");
}

void Model::Draw(const Camera& camera)
{
	for (auto& mesh : meshes)
	{
		mesh.Render(camera);
	}
}

void Model::LoadModel(const std::string& path,Shader* shader)
{
	static Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "[ERROR] ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene,shader);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene,Shader* shader)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene, shader));
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene,shader);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene,Shader* shader)
{
	vector <Vertex> vertexes(mesh->mNumVertices);
	vector <uint> indexes;
	vector <Texture> textures;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex& vert = vertexes[i];
		vert.position = {
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		};

		if(mesh->mNormals)
			vert.normal = {
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			};

		if (mesh->mTextureCoords[0])
		{
			vert.texCoord = { mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y }; // TODO use all the other uv textures (8 in total)
		}
		else vert.texCoord = { 0,0 };
	}

	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace& face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
			indexes.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = LoadMaterialTextures(mat, aiTextureType_DIFFUSE, SAMPLER_DIFFUSE_NAME);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = LoadMaterialTextures(mat, aiTextureType_SPECULAR, SAMPLER_SPECULAR_NAME);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	return Mesh(vertexes, indexes, textures,shader);
}

vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const string& typeName)
{
	vector<Texture> textures;
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString name;
		mat->GetTexture(type, i, &name);
		Texture texture(directory + "/" + name.C_Str());
		texture.type = typeName;
		textures.push_back(texture);
	}
	return textures;
}
