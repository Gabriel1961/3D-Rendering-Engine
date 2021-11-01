#pragma once
#include <Renderer.h>
#include <Common.h>
#include <Transform.h>
#include <string>
#include <vector>
#include "Camera.h"
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

class Mesh
{
public:

	VertexBuffer* vb = 0;
	IndexBuffer* ib = 0;
	VertexArray* va = 0;
	Shader* sh = 0;

	std::vector <Vertex> vertexes;
	std::vector <uint> indexes;
	std::vector <Texture> textures;

	glm::mat4 modelMat = glm::mat4(1);
	glm::mat4 viewMat = glm::mat4(1);

	Mesh(const std::vector<Vertex>& vertexes, const std::vector<uint>& indexes, const std::vector<Texture>& textures, Shader* shader);
	void Draw(const Camera& camera);
	Mesh(Mesh&& o) noexcept;
	~Mesh();
protected:
	void SetupMesh();
};

class Model
{
public:
	Model(const char* path,Shader* sh);
	void Draw(const Camera& camera);
	std::vector<Mesh> meshes;
	std::string directory;
protected:
	void LoadModel(const std::string& path,Shader* shader);
	void ProcessNode(aiNode* node, const aiScene* scene,Shader* shader);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, Shader* shader);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
};
