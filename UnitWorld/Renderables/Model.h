#pragma once

#include "Renderable.h"

#include <string_view>
#include <assimp/scene.h>

class Model : Renderable
{
public:
	Model(std::string_view FileName);
	virtual ~Model();

	void Load(std::string_view FileName);

	virtual void Submit(Renderer* renderer) override;

protected:
	void ProcessMesh(aiMesh *mesh, const aiScene *scene);
	void ProcessNode(aiNode *node, const aiScene *scene);
	void Clear();
protected:
	std::vector<Renderable*> m_Meshes;
	const aiScene* m_Scene;

	std::string m_FileName; // Debug
};