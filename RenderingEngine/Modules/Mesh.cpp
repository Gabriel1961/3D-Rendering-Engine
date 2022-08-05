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

#define SAMPLER_DIFFUSE_NAME string("diffuseTex")
#define SAMPLER_SPECULAR_NAME string("specularTex")
#define SAMPLER_AMBIENT_NAME string("ambientTex")
///  Mesh  ///

Mesh::Mesh(const std::vector<Vertex>& vertexes, const std::vector<uint>& indexes, const std::vector<Texture>& textures, Material* mat,Shader* shader)
	: vertexes(vertexes), indexes(indexes), textures(textures),sh(shader),mat(mat)
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
	viewMat = camera.GetViewMat();
	sh->Bind();
	mat->Bind(sh);
	sh->SetUniformMat4f("u_model", modelMat);
	sh->SetUniformMat4f("u_view", viewMat);
	sh->SetUniformMat4f("u_proj", camera.projMat);
	sh->SetUniformMat3f("u_normalMVMat", transpose(inverse(mat3(modelMat))));
	sh->SetUniform3f("u_viewPos", camera.position);

	//static Texture* t = new Texture("Scenes/BasicShapesScene/Assets/House/roof.jpg");
	if(textures.size())
	textures[0].Bind();
	Renderer::Draw(*va, *ib, *sh);
}


Mesh::Mesh(Mesh&& o)noexcept // [source of all evil]
{
	vb = o.vb;
	va = o.va;
	ib = o.ib;
	sh = o.sh;
	mat = o.mat;

	vertexes = move(o.vertexes);
	indexes = move(o.indexes);
	textures = move(o.textures);
	
	modelMat = o.modelMat;
	viewMat = o.viewMat;

	o.mat = 0;
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
	delete mat;
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
	Material* material = new Material();
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		
		aiColor4D color;
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->diffuse = vec4(color.r,color.g,color.b,color.a);
		mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->ambient = vec3(color.r,color.g,color.b);
		mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->specular = vec3(color.r,color.g,color.b);
		mat->Get(AI_MATKEY_SHININESS, material->shininess);
		
		vector<Texture> diffuseMaps = LoadMaterialTextures(mat, aiTextureType_DIFFUSE, SAMPLER_DIFFUSE_NAME);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		
		if(diffuseMaps.size() > 0)
			material->diffuseTex = &*textures.rbegin();

		vector<Texture> ambientMaps = LoadMaterialTextures(mat, aiTextureType_AMBIENT, SAMPLER_AMBIENT_NAME);
		textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());

		if (ambientMaps.size() > 0)
			material->ambientTex = &*textures.rbegin();
		
		vector<Texture> specularMaps = LoadMaterialTextures(mat, aiTextureType_SPECULAR, SAMPLER_SPECULAR_NAME);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		if (specularMaps.size() > 0)
			material->specularTex = &*textures.rbegin();

		

	}
	return Mesh(vertexes, indexes, textures,material,shader);
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
