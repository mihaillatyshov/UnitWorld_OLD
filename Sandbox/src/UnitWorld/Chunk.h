#pragma once

#include "Renderables/Mesh.h"
#include "Unit.h"
#include <unordered_map>

class Chunk
{
public:
	Chunk();
	~Chunk();
	void Submit(Renderer* renderer);
protected:
	int m_Units[16][256][16];
	std::string m_UnitsName[16][256][16];
	std::vector<std::string> m_Names;
	//std::vector<Unit> m_UnitVector;
	UGrass* m_TestUnit;
	std::vector<UnitPlane> m_PlaneVector;
	std::vector<std::shared_ptr<Renderable>> m_Renderables;
	std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_Textures;
	std::unordered_map<std::string, Material*> m_Materials;
};