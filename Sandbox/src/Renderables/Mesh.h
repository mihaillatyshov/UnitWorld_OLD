#pragma once

#include "Renderable.h"
#include <glm.hpp>
#include <gtx/transform.hpp>
#include "Renderers/Renderer.h"

//const static std::vector<glm::vec3> CubePositions
//{
//	// Front face
//	glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f),
//	glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f),
//
//	// Bottom face
//	glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, 0.5f),
//	glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f),
//
//	// Right face
//	glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),
//	glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
//
//	// Left face
//	glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f),
//	glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
//
//	// Back face
//	glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
//	glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),
//
//	// Top face
//	glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, -0.5f),
//	glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f)
//};

const static std::vector<glm::vec3> CubePositions
{
	// Front face  ok
	glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f),

	// Bottom face  ok
	glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f),

	// Right face  ok
	glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),

	// Left face  ok
	glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f),

	// Back face  ok
	glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),

	// Top face  
	glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f)
};

const static std::vector<glm::vec2> CubeUVs
{
	// Front face
	glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f),

	// Bottom face
	glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f),

	// Right face
	glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f),

	// Left face
	glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f),

	// Back face
	glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f),

	// Top face
	glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)
};

const static std::vector<glm::vec3> CubeNormals
{
	// Front face
	glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f),

	// Bottom face
	glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f),

	// Right face
	glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f),

	// Left face
	glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),

	// Back face
	glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),

	// Top face
	glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
};

class UnitPlane : public Renderable
{
public:
	enum PlaneMask
	{
		PlaneMask_Front,
		PlaneMask_Bot,
		PlaneMask_Right,
		PlaneMask_Left,
		PlaneMask_Back,
		PlaneMask_Top
	};

	UnitPlane(PlaneMask Id, const glm::vec3 &pos = glm::vec3(0.0f), Material* material = Material::s_StandartMaterial)
	{
		switch (Id)
		{
		case PlaneMask_Front:
			m_Indices = { 0, 1, 2, 3 };
			break;			   	   	   
		case PlaneMask_Bot:	   	   	   
			m_Indices = { 4, 5, 6, 7 };
			break;
		case PlaneMask_Right:
			m_Indices = { 8, 9, 10, 11 };
			break;
		case PlaneMask_Left:
			m_Indices = { 12, 13, 14, 15 };
			break;
		case PlaneMask_Back:
			m_Indices = { 16, 17, 18, 19 };
			break;
		case PlaneMask_Top:
			m_Indices = { 20, 21, 22, 23 };
			break;
		}
		m_Material = material;
		for (auto index : m_Indices)
		{
			m_Positions.push_back(CubePositions[index]);
			m_UVs.push_back(CubeUVs[index]);
			m_Normals.push_back(CubeNormals[index]);
			m_Tangents.push_back(CubeNormals[index]);
		}

		// 0 1 2  0 2 3
		m_Indices = {  0,  1,  2,  0,  2,  3 };
		m_Transform = glm::translate(pos);
		
	}

	virtual void Submit(Renderer* renderer) override
	{
		renderer->PushTransform(&m_Transform);
		Renderable::Submit(renderer);
		renderer->PopTransform();
	}
protected:
	glm::mat4 m_Transform;
};