#pragma once
#include <Renderer.h>
#include <Common.h>
#include <Transform.h>
#include <string>
#include <vector>
#include "Camera.h"
#include "Modules/Material/Material.h"
struct Vertex
{
	glm::vec3 position{};
	glm::vec3 normal{};
	glm::vec2 texCoord{};
};

class Mesh
{
public:

	shared_ptr<VertexBuffer> vb = 0;
	shared_ptr<IndexBuffer> ib = 0;
	shared_ptr<VertexArray> va = 0;
	shared_ptr<Shader> sh = 0;

	std::vector <Vertex> vertexes;
	std::vector <uint> indexes;
	std::vector <shared_ptr<Texture>> textures;
	
	shared_ptr<Material> mat;

	glm::mat4 modelMat = glm::mat4(1);
	glm::mat4 viewMat = glm::mat4(1);

	Mesh(const std::vector<Vertex>& vertexes,
		const std::vector<uint>& indexes,
		const std::vector<shared_ptr<Texture>>& textures,
		shared_ptr<Material> mat,
		shared_ptr<Shader> shader);
	Mesh() {}
	virtual void Render(const Camera& camera);
	Mesh(Mesh&& o) noexcept;
	~Mesh();
protected:
	void SetupMesh();
};

class Model
{
public:
	Model(const string& path,shared_ptr<Shader> sh);
	void Draw(const Camera& camera);
	std::vector<Mesh> meshes;
	std::string directory;
protected:
	void LoadModel(const std::string& path,shared_ptr<Shader> shader);
	void ProcessNode(aiNode* node, const aiScene* scene, shared_ptr<Shader> shader);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, shared_ptr<Shader> shader);
	std::vector<shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
};
