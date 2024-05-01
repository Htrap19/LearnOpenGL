#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <mutex>

static std::shared_ptr<Texture> s_PlainTexture;
static std::once_flag s_LoadOnce;

void Model::Load(const std::string& filePath)
{
	std::call_once(s_LoadOnce,
		[]()
		{
			s_PlainTexture = std::make_shared<Texture>();
			if (!s_PlainTexture->LoadFromFile("resources/textures/plain.png"))
				std::cerr << "Unable to load plain texture!" << std::endl;
		});

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "Unable to load model[" << filePath << "]: " 
			<< importer.GetErrorString() << std::endl;
		return;
	}

	LoadNode(scene->mRootNode, scene);
	LoadMaterials(scene);
}

void Model::Render()
{
	for (size_t i = 0; i < m_Meshes.size(); i++)
	{
		uint32_t textureIndex = m_MeshToTex[i];
		if (textureIndex < m_Textures.size() && m_Textures[textureIndex])
			m_Textures[textureIndex]->UseTexture(1);
		m_Meshes[i]->Render();
	}
}

void Model::Clear()
{
	m_Meshes.clear();
	m_Textures.clear();
	m_MeshToTex.clear();
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		LoadMesh(mesh, scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
		LoadNode(node->mChildren[i], scene);
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<float> vertices;
	std::vector<uint32_t> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(),
			{
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			});

		vertices.insert(vertices.end(),
			mesh->mTextureCoords[0] ?
				std::initializer_list{ mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y } :
				std::initializer_list{ 0.0f, 0.0f } );
		
		vertices.insert(vertices.end(),
			{
				-mesh->mNormals[i].x,
				-mesh->mNormals[i].y,
				-mesh->mNormals[i].z
			});
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	auto ourMesh = std::make_shared<Mesh>();
	ourMesh->Create(vertices, indices);
	m_Meshes.push_back(ourMesh);
	m_MeshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene)
{
	m_Textures.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];
		m_Textures[i] = s_PlainTexture;
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == aiReturn_SUCCESS)
			{
				auto p = std::string(path.data);
				size_t lastSlash = p.rfind('\\');
				std::string texPath = "resources/textures/" + p.substr(lastSlash + 1);
				auto texture = std::make_shared<Texture>();
				if (!texture->LoadFromFile(texPath))
				{
					std::cerr << "Failed to load texture: " << texPath << std::endl;
					continue;
				}
				
				m_Textures[i] = texture;
			}
		}
	}
}
