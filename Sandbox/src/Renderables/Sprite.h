#pragma once

#include "Renderable.h"

class Sprite : public Renderable
{
public:
	Sprite(glm::vec2 position, glm::vec2 size, Material* material = Material::s_StandartMaterial);
	virtual void Submit(Renderer* renderer) override;

	void ImGuiDraw();
protected:
	glm::vec2 m_Translation;
	float     m_Rotation;
	glm::vec2 m_Scaling;
	float m_Depth;
	//glm::vec3 m_Transformation;
};