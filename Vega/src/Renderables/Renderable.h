#pragma once

#include <vector>

#include <glm.hpp>

#include "Core/Material.h"


class Renderer;

//class Material
//{
//public:
//	Texture2D* Diffuse1 = nullptr;
//	//glm::vec4  DiffuseColor1;
//	Texture2D* Diffuse2 = nullptr;
//	//glm::vec4  DiffuseColor2;
//	Texture2D* Diffuse3 = nullptr;
//	//glm::vec4  DiffuseColor3;
//
//public:
//	const Material* GetStandartMaterial();
//	void SetStandertMaterial();
//protected:
//	static Material* s_StandartMaterial;
//};



class Renderable
{
public:
	Renderable() = default;
	virtual ~Renderable() = default;

	virtual void Submit(Renderer* renderer);

	inline const std::vector<glm::vec3> &GetPositions() const { return m_Positions; }
	inline const std::vector<glm::vec2> &GetUVs() const { return m_UVs; }
	inline const std::vector<glm::vec3> &GetNormals() const { return m_Normals; }
	inline const std::vector<glm::vec3> &GetTangents() const { return m_Tangents; }
	inline const std::vector<uint32_t> &GetIndices() const { return m_Indices; }
	inline const Material* GetMaterial() const { return m_Material; }

	inline std::vector<glm::vec3> *GetPositionsPtr() { return &m_Positions; }
	inline std::vector<glm::vec2> *GetUVsPtr() { return &m_UVs; }
	inline std::vector<glm::vec3> *GetNormalsPtr() { return &m_Normals; }
	inline std::vector<glm::vec3> *GetTangentsPtr() { return &m_Tangents; }
	inline std::vector<uint32_t> *GetIndicesPtr() { return &m_Indices; }

	inline void SetPositions(const std::vector<glm::vec3>& positions) { m_Positions = positions; }
	inline void SetUVs(const std::vector<glm::vec2>& uvs) { m_UVs = uvs; }
	inline void SetNormals(const std::vector<glm::vec3>& normals) { m_Normals = normals; }
	inline void SetTangents(const std::vector<glm::vec3>& tangents) { m_Tangents = tangents; }
	inline void SetIndices(const std::vector<uint32_t>& indices) { m_Indices = indices; }

	void FillStandardUVs();
	void FillStandardNormals();
	void FillStandardTangents();
	void FillStandardIndices();
	void CalcTangents();
	
	inline void SetMaterial(Material* material) { m_Material = material; };
protected:
	//void CalcTangent();
protected:
	std::vector<glm::vec3> m_Positions;
	std::vector<glm::vec2> m_UVs;
	std::vector<glm::vec3> m_Normals;
	std::vector<glm::vec3> m_Tangents;
	
	std::vector<uint32_t> m_Indices;
	Material* m_Material;
};