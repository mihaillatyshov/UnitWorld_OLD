#include "SharedLayer.h"

#include <imgui.h>
#include <glfw3.h>

#include "Player/Camera.h"

namespace LM
{

	SharedLayer::SharedLayer()
	{

	}

	SharedLayer::~SharedLayer()
	{

	}

	void SharedLayer::OnAttach()
	{
		m_Player = new Player(new Camera);
		m_Lights = new Lights();
	}

	void SharedLayer::OnDetach()
	{

	}

	void SharedLayer::OnUpdate()
	{
		m_Player->OnUpdate();

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		if (ImGui::IsKeyPressed(GLFW_KEY_E))
			m_EditCamera = !m_EditCamera;

		if (m_EditCamera)
		{
			m_Player->GetCamera()->addRotation(glm::vec2(io.MouseDelta.x / 4.0f, -io.MouseDelta.y / 4.0f));
			//m_Camera.addRotation(glm::vec2(io.MouseDelta.x / 4.0f, -io.MouseDelta.y / 4.0f));
		}
		//m_Lights->Angle.x += 0.5f;
	}

	void SharedLayer::OnImGuiRender()
	{
		if (ImGui::Begin("Camera"))
		{
			Camera& m_Camera = *m_Player->GetCamera();
			glm::vec3 gPos = m_Camera.getPosition();
			glm::vec2 gAng = m_Camera.getAngle();
			ImGui::DragFloat3("Position", &gPos.x, 0.01f);
			ImGui::DragFloat2("Angle", &gAng.x, 0.1f);
			m_Camera.SetPosition(gPos);
			m_Camera.SetAngle(gAng);

		}
		ImGui::End();


		if (ImGui::Begin("Lights"))
		{
			ImGui::DragFloat2("Angle", &m_Lights->Angle.x, 0.1f);
			ImGui::ColorEdit3("Ambient", &m_Lights->Ambient.x);
			ImGui::ColorEdit3("Diffuse", &m_Lights->Diffuse.x);
			ImGui::ColorEdit3("Specular", &m_Lights->Specular.x);

		}
		ImGui::End();

	}

}