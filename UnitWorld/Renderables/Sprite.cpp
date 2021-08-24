#include "Sprite.h"

#include "Renderers/Renderer.h"
#include <gtx/transform.hpp>
#include <imgui.h>

Sprite::Sprite(glm::vec2 position, glm::vec2 size, Material* material)
{
	//m_Transformation = glm::vec3(position, 0.0f);
	m_Depth = 0.0f;
	m_Translation = position;
	m_Rotation = 0.0f;
	m_Scaling = glm::vec2(1.0f);

	m_Material = material;

	m_Positions = { glm::vec3(-size.x / 2.0f, -size.y / 2.0f, 0.0f),
					glm::vec3( size.x / 2.0f, -size.y / 2.0f, 0.0f),
					glm::vec3( size.x / 2.0f,  size.y / 2.0f, 0.0f),
					glm::vec3(-size.x / 2.0f,  size.y / 2.0f, 0.0f) };

	m_UVs = { glm::vec2(0.0f, 0.0f), 
			  glm::vec2(1.0f, 0.0f), 
			  glm::vec2(1.0f, 1.0f), 
			  glm::vec2(0.0f, 1.0f) };

	m_Normals = { glm::vec3(0.0f, 0.0f, 1.0f), 
				  glm::vec3(0.0f, 0.0f, 1.0f), 
				  glm::vec3(0.0f, 0.0f, 1.0f), 
				  glm::vec3(0.0f, 0.0f, 1.0f) };

	m_Tangents = { glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3() };

	m_Indices = { 0, 1, 2 , 0, 2, 3 };
}


void Sprite::Submit(Renderer* renderer)
{
	glm::mat4 PosTransform = glm::translate(glm::vec3(m_Translation, m_Depth));
	glm::mat4 RotTransform = glm::rotate(glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, -1.0f));
	glm::mat4 SclTransform = glm::scale(glm::vec3(m_Scaling, 0.0f));

	glm::mat4 Result = PosTransform * RotTransform * SclTransform;

	renderer->PushTransform(&Result);
	Renderable::Submit(renderer);
	renderer->PopTransform();
}


void Sprite::ImGuiDraw()
{
	ImGui::PushID(this);
	ImGui::DragFloat2("Position", &m_Translation.x, 0.01f);
	ImGui::DragFloat ("Rotation", &m_Rotation,      0.1f );
	ImGui::DragFloat2("Scaling",  &m_Scaling.x,     0.01f);
	ImGui::DragFloat ("Depth",    &m_Depth,         0.01f);
	ImGui::PopID();
}
