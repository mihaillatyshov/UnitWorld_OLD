#include "Chunk.h"

#include <cmath>
#include "SimplexNoise/SimplexNoise.h"
#include "Renderers/Renderer.h"

#include <math.h>

Chunk::Chunk()
{

	int m_ChunkCordX = 0;
	int m_ChunkCordZ = 0;
	for (int ix = 0; ix < 16; ix++)
	{
		for (int iz = 0; iz < 16; iz++)
		{
			int TopYUnit = floor(abs(10 * SimplexNoise::noise((ix + m_ChunkCordX * 16) * 0.01,
				(iz + m_ChunkCordZ * 16) * 0.01))) + 20;
			//TopZUnit = 30;
			//m_UnitVector.push_back(Unit(glm::vec3(ix, TopYUnit, iz)));
			for (int iy = 0; iy < 256; iy++)
			{
				if (iy < TopYUnit - 3) 
				{
					m_Units[ix][iy][iz] = 3;
					m_UnitsName[ix][iy][iz] = "Stone";
					//m_Units /*Class*/ = new StoneUnit;
					//UnitId[ix][iy][iz] = 1;
				}
				else if (iy < TopYUnit) 
				{
					m_Units[ix][iy][iz] = 2;
					m_UnitsName[ix][iy][iz] = "Dirth";
					//UnitId[ix][iy][iz] = 1;
				}
				else if (iy == TopYUnit)
				{
					m_Units[ix][iy][iz] = 1;
					m_UnitsName[ix][iy][iz] = "Grass";
					//UnitId[ix][iy][iz] = 1;
				}
				else
				{
					m_Units[ix][iy][iz] = 0;
					m_UnitsName[ix][iy][iz] = "Air";
				}


				//UnitMask[ix][iy][iz] = 0;
			}
		}
	}
	//m_Units[5][28][5] = 1;
	//m_UnitsName[5][28][5] = "Grass";




	m_Textures["Dirth_Top"] = std::make_shared<Texture2D>("./res/Textures/UW/dirth/16/Top.png", Texture2D::NEAREST);
	m_Textures["Dirth_Side"] = std::make_shared<Texture2D>("./res/Textures/UW/dirth/16/Side.png", Texture2D::NEAREST);
	m_Textures["Dirth_Bot"] = std::make_shared<Texture2D>("./res/Textures/UW/dirth/16/Bot.png", Texture2D::NEAREST);
	m_Textures["Stone"] = std::make_shared<Texture2D>("./res/Textures/UW/stone/Stone.png", Texture2D::NEAREST);

	m_Materials["Grass_Top"] = new Material(m_Textures["Dirth_Top"], glm::vec4(1.0f));
	m_Materials["Grass_Side"] = new Material(m_Textures["Dirth_Side"], glm::vec4(1.0f));
	m_Materials["Grass_Bot"] = new Material(m_Textures["Dirth_Bot"], glm::vec4(1.0f));

	m_Materials["Dirth_Top"] = new Material(m_Textures["Dirth_Bot"], glm::vec4(1.0f));
	m_Materials["Dirth_Side"] = m_Materials["Dirth_Top"];
	m_Materials["Dirth_Bot"] = m_Materials["Dirth_Top"];

	m_Materials["Stone_Top"] = new Material(m_Textures["Stone"], glm::vec4(1.0f));
	m_Materials["Stone_Side"] = m_Materials["Stone_Top"];
	m_Materials["Stone_Bot"] = m_Materials["Stone_Top"];

	//m_Materials["Air_Top"] = new Material(m_Textures["Stone"], glm::vec4(1.0f));
	//m_Materials["Air_Side"] = new Material(m_Textures["Stone"], glm::vec4(1.0f));
	//m_Materials["Air_Bot"] = new Material(m_Textures["Stone"], glm::vec4(1.0f));

	Unit::s_Textures["Dirth"] = std::make_shared<Texture2D>("./res/Textures/UW/dirth/16/Dirth.png", Texture2D::NEAREST);
	Unit::s_Materials["Grass"] = new Material(Unit::s_Textures["Dirth"], glm::vec4(1.0f));
	m_TestUnit = new UGrass;

	auto meshes = m_TestUnit->GetMeshes(glm::ivec3(5, 28, 5), UnitMask_Front | UnitMask_Bot | UnitMask_Right | UnitMask_Left | UnitMask_Back | UnitMask_Top);
	for (auto& mesh : meshes)
	{ 
		m_Renderables.push_back(mesh);
	}


	for (int ix = 0; ix < 16; ix++) 
	{
		for (int iz = 0; iz < 16; iz++) 
		{
			for (int iy = 0; iy < 256; iy++) 
			{
				if (m_Units[ix][iy][iz] != 0) 
				{

					//uint8_t mask = 0;

					//LOGI("Unit ID: " << m_Units[ix][iy][iz]->GetId());

					// Front mask
					if (ix == 0) 
					{
						m_PlaneVector.push_back(UnitPlane(UnitPlane::PlaneMask_Right, glm::vec3(ix, iy, iz), m_Materials[m_UnitsName[ix][iy][iz] + std::string("_Side")]));
					}
					else if (m_Units[ix - 1][iy][iz] == 0) 
					{
						//switch (m_Units[ix][iy][iz])
						m_PlaneVector.push_back(UnitPlane(UnitPlane::PlaneMask_Right, glm::vec3(ix, iy, iz), m_Materials[m_UnitsName[ix][iy][iz] + std::string("_Side")]));
						//mask |= PlaneDirectionMask::MASK_RIGHT;
					}

					// Top mask
					if (iy == 255) 
					{
						m_PlaneVector.push_back(UnitPlane(UnitPlane::PlaneMask_Top, glm::vec3(ix, iy, iz), m_Materials[m_UnitsName[ix][iy][iz] + std::string("_Top")]));
						//mask |= PlaneDirectionMask::MASK_TOP;
					}
					else if (m_Units[ix][iy + 1][iz] == 0) 
					{
						m_PlaneVector.push_back(UnitPlane(UnitPlane::PlaneMask_Top, glm::vec3(ix, iy, iz), m_Materials[m_UnitsName[ix][iy][iz] + std::string("_Top")]));
						//mask |= PlaneDirectionMask::MASK_TOP;
					}

					// Backside mask
					if (ix == 15) {}
					else if (m_Units[ix + 1][iy][iz] == 0) 
					{
						m_PlaneVector.push_back(UnitPlane(UnitPlane::PlaneMask_Left, glm::vec3(ix, iy, iz), m_Materials[m_UnitsName[ix][iy][iz] + std::string("_Side")]));
						//mask |= PlaneDirectionMask::MASK_LEFT;
					}

					// Bottom mask
					if (iy == 0) 
					{
					}
					else if (m_Units[ix][iy - 1][iz] == 0) 
					{
						m_PlaneVector.push_back(UnitPlane(UnitPlane::PlaneMask_Bot, glm::vec3(ix, iy, iz), m_Materials[m_UnitsName[ix][iy][iz] + std::string("_Bot")]));
						//mask |= PlaneDirectionMask::MASK_BOTTOM;
					}

					// Left mask
					if (iz == 0) {}
					else if (m_Units[ix][iy][iz - 1] == 0) 
					{
						m_PlaneVector.push_back(UnitPlane(UnitPlane::PlaneMask_Front, glm::vec3(ix, iy, iz), m_Materials[m_UnitsName[ix][iy][iz] + std::string("_Side")]));
						//mask |= PlaneDirectionMask::MASK_FRONT;
					}

					// Right mask
					if (iz == 15) {}
					else if (m_Units[ix][iy][iz + 1] == 0) 
					{
						m_PlaneVector.push_back(UnitPlane(UnitPlane::PlaneMask_Back, glm::vec3(ix, iy, iz), m_Materials[m_UnitsName[ix][iy][iz] + std::string("_Side")]));
						//mask |= PlaneDirectionMask::MASK_BACK;
					}

					
				}
			}
		}
	}
}


Chunk::~Chunk()
{
	for (auto& [name, material] : m_Materials)
	{
		//delete material;
	}
}


void Chunk::Submit(Renderer* renderer)
{
	//for (auto& unit : m_UnitVector)
	//{
	//	unit.Submit(renderer);
	//}
	for (auto& plane : m_PlaneVector)
	{
		plane.Submit(renderer);
	}
	for (auto& mesh : m_Renderables)
	{
		mesh->Submit(renderer);
	}
}
