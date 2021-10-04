#include "Model.h"

#include <iostream>

#include "Renderers/Renderer.h"
#include "Debug/ConsoleLog.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <gtx/transform.hpp>

Model::Model(std::string_view FileName)
{
	Load(FileName);
}

Model::~Model()
{
	for (auto& mesh : m_Meshes)
	{
		delete mesh;
	}
}


void Model::Load(std::string_view FileName)
{
	Clear();
	m_FileName = FileName;

	Assimp::Importer importer;
	m_Scene = importer.ReadFile(FileName.data(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_GenUVCoords);
	if (!m_Scene || m_Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_Scene->mRootNode)
	{
		LOGI(importer.GetErrorString());
		return;
	}

	// TODO: ?
	//SetMat4Assimp(m_GlobalInverseTransform, m_Scene->mRootNode->mTransformation);
	//m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);

	ProcessNode(m_Scene->mRootNode, m_Scene);
}


void Model::Submit(Renderer* renderer)
{
	//renderer->PushTransform(glm::mat4(1.0f));

	for (auto& renderable : m_Meshes)
	{
		renderable->Submit(renderer);
	}

	//renderer->PopTransform();
}


void Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<uint32_t> indices;

	for (unsigned i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;

	for (unsigned i = 0; i < mesh->mNumVertices; i++)
	{
		positions.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
		normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
		texCoords.push_back(mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f));
	}

	// TODO:
	// LoadBones();

	m_Meshes.push_back(new Renderable);
	m_Meshes.back()->SetPositions(positions);
	m_Meshes.back()->SetUVs(texCoords);
	m_Meshes.back()->SetNormals(normals);
	m_Meshes.back()->FillStandardTangents();
	m_Meshes.back()->SetIndices(indices);
	m_Meshes.back()->SetMaterial(Material::s_StandartMaterial);


}


void Model::ProcessNode(aiNode *node, const aiScene *scene)
{
	for (unsigned i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}

	for (unsigned i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void Model::Clear()
{

}