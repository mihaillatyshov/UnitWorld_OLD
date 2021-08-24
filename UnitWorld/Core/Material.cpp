#include "Material.h"

Material* Material::s_StandartMaterial = new Material;


void Material::SetStandertMaterial(Material* material)
{
	if (material)
	{
		delete s_StandartMaterial;
		s_StandartMaterial = material;
	}
}

bool Material::HasTextures() const
{
	//Texture* t1 = nullptr;
	//Texture* t2 = nullptr;
	//Texture* t3 = nullptr;
	//
	//return t1 || t2 || t3;

	//(bool)Diffuse

	if (Diffuse /* || Specular || Normal */)
		return true;

	return false;

	//return (bool)Diffuse /* || Specular || Normal */;
}
