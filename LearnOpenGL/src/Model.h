#pragma once

#include "Texture.h"
#include "Mesh.h"

#include <vector>
#include <string>
#include <memory>

// Forward declaration
struct aiNode;
struct aiMesh;
struct aiScene;

class Model
{
public:
	Model() = default;

	void Load(const std::string& filePath);
	void Render();
	void Clear();

private:
	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene);

private:
	std::vector<std::shared_ptr<Mesh>> m_Meshes;
	std::vector<std::shared_ptr<Texture>> m_Textures;
	std::vector<uint32_t> m_MeshToTex;
};

