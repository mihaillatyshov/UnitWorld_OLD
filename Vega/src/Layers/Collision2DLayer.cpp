#include "Collision2DLayer.h"

#include "Physics/CollisionTest.h"
#include "Physics/Circle.h"
#include "Physics/AABB.h"

#include <gl/glew.h>
#include <imgui.h>

Collision2DLayer::Collision2DLayer()
{
	DeltaTime = 0.002f;
}


Collision2DLayer::~Collision2DLayer()
{

}


void Collision2DLayer::OnAttach()
{
	//m_Objects.push_back(new Circle( { glm::vec2(-5.0f, -6.0f), glm::vec2( 1.0f,  1.0f), 1.0f }, { 1.0f } ));
	//m_Objects.push_back(new Circle( { glm::vec2( 4.0f,  4.0f), glm::vec2(-1.0f, -1.0f), 1.0f }, { 1.0f } ));

	
	m_Objects.push_back(new Circle( { glm::vec2(0.0f, 0.0f),                                                         glm::vec2(10.0f,                             0.0f                            ), 0.001f }, { 1.0f } ));
	m_Objects.push_back(new Circle( { glm::vec2(cos(glm::radians(45.0f)) * 2.0f , -sin(glm::radians(45.0f)) * 2.0f), glm::vec2(-cos(glm::radians(45.0f)) * 10.0f, sin(glm::radians(45.0f)) * 10.0f), 0.001f }, { 1.0f } ));

	//m_Objects.push_back(new AABB( { glm::vec2(-5.0f, -6.0f), glm::vec2( 1.0f,  1.0f), 1.0f }, { glm::vec2(1.0f, 1.0f) } ));
	//m_Objects.push_back(new AABB( { glm::vec2( 6.0f,  6.0f), glm::vec2(-0.06f, -1.0f), 1.0f }, { glm::vec2(1.0f, 1.0f) } ));
	
	//m_Objects.push_back(new AABB( { glm::vec2(-0.0f, -5.0f), glm::vec2( 0.0f,  0.0f), 0.0f }, { glm::vec2(20.0f, 1.0f) } ));
}

void Collision2DLayer::OnDetach()
{
	for (auto& obj : m_Objects)
	{
		delete obj;
	}
	m_Objects.clear();
}


void Collision2DLayer::OnUpdate()
{
	Manifold manifold;


	for (auto& obj : m_Objects)
	{
		if (obj->GetInvMass() != 0.0f)
		{
			//obj->AddVelocity(glm::vec2(0.0f, -1.8f * DeltaTime));
		}
	}

	for (int i = 0; i < m_Objects.size() - 1; i++)
	{
		for (int j = i + 1; j < m_Objects.size(); j++)
		{
			if (m_Objects[i]->GetInvMass() == 0.0f && m_Objects[j]->GetInvMass() == 0.0f)
				continue;

			manifold.A = m_Objects[i];
			manifold.B = m_Objects[j];
			if (CollisionTest::Intersect(&manifold))
			{
				// Resolve collision
				CollisionTest::ResolveCollision(&manifold);

				//CollisionTest::Solve(&manifold, DeltaTime);
			}
		}
	}
	
	for (auto& obj : m_Objects)
	{
		obj->Integrate(DeltaTime);
	}

	glUseProgram(0);
	glPushMatrix();
	glOrtho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f);
	for (auto& obj : m_Objects)
	{
		obj->Draw();
	}
	glPopMatrix();
}


void Collision2DLayer::OnImGuiRender()
{
	if (ImGui::Begin("Collision2DLayer"))
	{
		ImGui::DragFloat("DeltaTime", &DeltaTime, 0.001f, 0.00001f, FLT_MAX);
		if (ImGui::Button("Reset"))
		{
			OnDetach();
			OnAttach();
		}
		for (auto& obj : m_Objects)
		{
			ImGui::PushID(obj);
			ImGui::Text("Position: %06.6f %06.6f", obj->GetPosition().x, obj->GetPosition().y);
			ImGui::Text("Velocity: %06.6f %06.6f", obj->GetVelocity().x, obj->GetVelocity().y);
			ImGui::Separator();
			ImGui::PopID();
		}
	}
	ImGui::End();
}