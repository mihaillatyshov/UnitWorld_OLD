#include "Renderable.h"

#include "Renderers/Renderer.h"


void Renderable::Submit(Renderer* renderer)
{
	renderer->Submit(this);
}

//Material* Material::s_StandartMaterial = new Material;

void Renderable::FillStandardUVs()
{
	m_UVs.clear();
	for (uint32_t i = 0; i < m_Positions.size(); i++)
	{
		m_UVs.push_back(glm::vec2(0.0f));
	}
}


void Renderable::FillStandardNormals()
{
	m_Normals.clear();
	for (uint32_t i = 0; i < m_Positions.size(); i++)
	{
		m_Normals.push_back(glm::vec3(0.0f));
	}
}


void Renderable::FillStandardTangents()
{
	m_Tangents.clear();
	for (uint32_t i = 0; i < m_Positions.size(); i++)
	{
		m_Tangents.push_back(glm::vec3(0.0f));
	}
}


void Renderable::FillStandardIndices()
{
	m_Indices.clear();
	for (uint32_t i = 0; i < m_Positions.size(); i++)
	{
		m_Indices.push_back(i);
	}
}


void Renderable::CalcTangents()
{
	// TODO:
}
