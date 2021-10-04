#include "Player.h"

#include "Camera.h"

#include <imgui.h>
#include <glfw3.h>

#include <iostream>

#include "Core/Timer.h"

Player::Player(Camera* camera)
	: m_Camera(camera), m_Position(glm::vec3(0.0f))
{
	s_Player = this;
}

void Player::OnUpdate()
{
	//m_Camera->getForward();
	m_Camera->getAngle();
	glm::vec3 direction = glm::vec3();

	
	if (ImGui::IsKeyDown(GLFW_KEY_A))
		direction.x -= 1.0f;
	if (ImGui::IsKeyDown(GLFW_KEY_D))
		direction.x += 1.0f;
	
	if (ImGui::IsKeyDown(GLFW_KEY_W))
		direction.z -= 1.0f;
	if (ImGui::IsKeyDown(GLFW_KEY_S))
		direction.z += 1.0f;


	direction = glm::rotate(glm::radians(m_Camera->getAngle().x), glm::vec3(0.0f, -1.0f, 0.0f)) *
				glm::rotate(glm::radians(m_Camera->getAngle().y), glm::vec3(1.0f,  0.0f, 0.0f)) * glm::vec4(direction, 0.0f);

	if (ImGui::IsKeyDown(GLFW_KEY_SPACE))
		direction.y += 1.0f;
	if (ImGui::IsKeyDown(GLFW_KEY_LEFT_SHIFT))
		direction.y -= 1.0f;

	//static double Time = ImGui::GetTime();
	//double NewTime = ImGui::GetTime();
	//
	//float DeltaTime = NewTime - Time;
	//Time = NewTime;

	if (glm::length(direction) < 0.0001)
		return;
	
	direction = glm::normalize(direction);

	m_Position += direction * float(Timer::GetDeltaTime()) * m_Speed;

	m_Camera->SetPosition(m_Position);
	
}
