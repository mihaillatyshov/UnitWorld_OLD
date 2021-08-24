#pragma once

#include <vector>

#include "Renderables/Renderable.h"
#include <memory>
#include <unordered_map>

class Mesh3D : public Renderable
{
public:
	virtual void Submit(Renderer* renderer) override
	{
		renderer->PushTransform(&m_Transform);
		Renderable::Submit(renderer);
		renderer->PopTransform();
	}
public:
	glm::mat4 m_Transform;
};

enum UnitMask : uint32_t
{
	UnitMask_Front = BIT(0),
	UnitMask_Bot   = BIT(1),
	UnitMask_Right = BIT(2),
	UnitMask_Left  = BIT(3),
	UnitMask_Back  = BIT(4),
	UnitMask_Top   = BIT(5)
};

class Unit
{
public:
	virtual std::vector<std::shared_ptr<Renderable>> GetMeshes(const glm::ivec3 &position, uint32_t mask) const = 0;
	virtual bool UnusedTopMask() const { return false; }
	virtual bool UnusedBotMask() const { return false; }
	virtual bool UnusedLeftMask() const { return false; }
	virtual bool UnusedRightMask() const { return false; }
	virtual bool UnusedFrontMask() const { return false; }
	virtual bool UnusedBackMask() const { return false; }

public:
	static inline std::unordered_map<std::string, std::shared_ptr<Texture2D>> s_Textures;
	static inline std::unordered_map<std::string, Material*> s_Materials;
};


class UAir : public Unit
{
public:
	virtual std::vector<std::shared_ptr<Renderable>> GetMeshes(const glm::ivec3 &position, uint32_t mask) const override { return {}; }
	virtual bool UnusedTopMask() const override { return true; }
	virtual bool UnusedBotMask() const override { return true; }
	virtual bool UnusedLeftMask() const override { return true; }
	virtual bool UnusedRightMask() const override { return true; }
	virtual bool UnusedFrontMask() const override { return true; }
	virtual bool UnusedBackMask() const override { return true; }
};

const static std::vector<glm::vec2> GrassUVs
{
	// Front face
	glm::vec2(0.5f, 0.5f), glm::vec2(1.0f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec2(0.5f, 1.0f),

	// Bottom face
	glm::vec2(0.0f, 0.0f), glm::vec2(0.5f, 0.0f), glm::vec2(0.5f, 0.5f), glm::vec2(0.0f, 0.5f),

	// Right face
	glm::vec2(0.5f, 0.5f), glm::vec2(1.0f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec2(0.5f, 1.0f),

	// Left face
	glm::vec2(0.5f, 0.5f), glm::vec2(1.0f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec2(0.5f, 1.0f),

	// Back face
	glm::vec2(0.5f, 0.5f), glm::vec2(1.0f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec2(0.5f, 1.0f),

	// Top face
	glm::vec2(0.0f, 0.5f), glm::vec2(0.5f, 0.5f), glm::vec2(0.5f, 1.0f), glm::vec2(0.0f, 1.0f)
};

class UGrass : public Unit
{
public:
	virtual std::vector<std::shared_ptr<Renderable>> GetMeshes(const glm::ivec3 &position, uint32_t mask) const override 
	{
		std::vector<std::shared_ptr<Renderable>> result;
		for (int i = 0; i < 6; i++)
		{
			if (!(BIT(i)))
				continue;
			
			std::shared_ptr<Mesh3D> mesh = std::make_shared<Mesh3D>();
			result.push_back(mesh);

			std::vector<uint32_t> indices;

			switch (BIT(i))
			{
			case UnitMask_Front:
				indices = { 0, 1, 2, 3 };
				break;
			case UnitMask_Bot:
				indices = { 4, 5, 6, 7 };
				break;
			case UnitMask_Right:
				indices = { 8, 9, 10, 11 };
				break;
			case UnitMask_Left:
				indices = { 12, 13, 14, 15 };
				break;
			case UnitMask_Back:
				indices = { 16, 17, 18, 19 };
				break;
			case UnitMask_Top:
				indices = { 20, 21, 22, 23 };
				break;
			}

			mesh->SetMaterial(Unit::s_Materials["Grass"]);
			for (auto index : indices)
			{
				mesh->GetPositionsPtr()->push_back(CubePositions[index]);
				mesh->GetUVsPtr()->push_back(GrassUVs[index]);
				mesh->GetNormalsPtr()->push_back(CubeNormals[index]);
				mesh->GetTangentsPtr()->push_back(CubeNormals[index]);
			}
			mesh->m_Transform = glm::translate((glm::vec3)position);
		

			// 0 1 2  0 2 3
			*mesh->GetIndicesPtr() = {  0,  1,  2,  0,  2,  3 };
		}

		return result;
	}
};