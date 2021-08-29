#include "GeometryShaderLayer.h"

#include <imgui.h>
#include <gtx/transform.hpp>

#define BIT(x) (1 << x)
#define BIT_TOP BIT(0)
#define BIT_BOT BIT(1)
#define BIT_LEFT BIT(2)
#define BIT_RIGHT BIT(3)
#define BIT_FRONT BIT(4)
#define BIT_BACK BIT(5)

namespace LM
{



	GeometryShaderLayer::GeometryShaderLayer()
	{
		
	}


	void GeometryShaderLayer::OnAttach()
	{
		const uint32_t MAX_POINTS_COUNT = 100000;

		m_Shader = std::make_unique<Shader>(
			ShaderPaths("./res/Shaders/Geometry/Vert.vert", "./res/Shaders/Geometry/Frag.frag", "./res/Shaders/Geometry/Geom.geom")
			);
		
		m_Camera = std::make_unique<Camera>();
		m_Camera->SetPosition(glm::vec3(1.0f, 0.25f, 2.5f));

		BufferLayout layout{ { ShaderDataType::Float3, "Position"       },
							 { ShaderDataType::Int,    "MaterialID"     }, 
							 { ShaderDataType::Int,    "DisplayedFaces" } };

		m_VBO = std::make_shared<VertexBuffer>(layout.GetStride() * MAX_POINTS_COUNT);
		m_VBO->SetLayout(layout);

		m_IBO = std::make_shared<IndexBuffer>(MAX_POINTS_COUNT);

		m_VAO = std::make_shared<VertexArray>();
		m_VAO->AddVertexBuffer(m_VBO);
		m_VAO->SetIndexBuffer(m_IBO);

		int mask = BIT(0) | BIT(1) | BIT(2); 
		m_Points.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), 0, mask);
		m_Indices.push_back(0);
	}


	void GeometryShaderLayer::OnDetach()
	{

	}


	void GeometryShaderLayer::OnUpdate()
	{
		m_Shader->Enable();

		m_Shader->SetUniformMat4("u_ProjMat", glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -10.0f, 10.0f));
		if (m_IsPerspective)
			m_Shader->SetUniformMat4("u_ProjMat", glm::perspective(80.0f, 16.0f / 9.0f, 0.01f, 100.0f));
		m_Shader->SetUniformMat4("u_ViewMat", m_Camera->GetMaxtrix());


		//m_Projection = Projection(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f));
		//m_Projection = Projection(glm::perspective(80.0f, 16.0f / 9.0f, 0.01f, 100.0f));


		m_VBO->SetData(m_Points.data(), m_VBO->GetLayout().GetStride() * m_Points.size());
		m_IBO->SetIndices(m_Indices.data(), m_Indices.size());


		m_VAO->Bind();
		m_IBO->Bind();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDrawElements(GL_POINTS, m_IBO->GetCount(), GL_UNSIGNED_INT, NULL);
	}


	void GeometryShaderLayer::OnImGuiRender()
	{
		if (ImGui::Begin("GeometryShaderLayer"))
		{
			//static glm::vec4 Param = glm::vec4(10.0f, 2.0f, 10.0f, 1.0f);
			//ImGui::DragFloat4;
			//glm::perspective(80.0f, 16.0f / 9.0f, 0.01f, 100.0f) * Param;
			if (ImGui::Button("Recompile shader"))
			{
				m_Shader->Recompile();
			}
			
			ImGui::Checkbox("Is Perspective", &m_IsPerspective);

			
			ImGui::Separator();

			glm::vec3 pos = m_Camera->getPosition();
			ImGui::DragFloat3("Camera position", &pos[0], 0.1f);
			m_Camera->SetPosition(pos);

			glm::vec2 ang = m_Camera->getAngle();
			ImGui::DragFloat2("Camera angle", &ang[0], 0.5f);
			m_Camera->SetAngle(ang);

			ImGui::Separator();


			if (ImGui::Button("Create New Point"))
			{
				m_Points.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), m_Points.size(), BIT_TOP);
				m_Indices.push_back(m_Indices.size());
			}
			
			ImGui::Spacing(); 
			ImGui::Spacing();
			
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
			ImGui::BeginChild("Child1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.95f, 0.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
			for (int i = 0; i < m_Points.size(); i++)
			{
				ImGui::PushID(i);

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Object %4d      ", i);
				ImGui::DragFloat3("Pos", &m_Points[i].Position.x, 0.01f);
				ImGui::DragInt("MID", &m_Points[i].MaterialID);
				
				ImGuiDrawMask(&m_Points[i]);

				if (ImGui::Button("Remove"))
				{
					m_Points.erase(m_Points.begin() + i);
					m_Indices.pop_back();
				}

				ImGui::Separator();
				ImGui::Spacing();
				ImGui::PopID();
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}

	
	void GeometryShaderLayer::ImGuiDrawMask(Point* point)
	{
		std::string names[] = { "Top", "Bot", "Left", "Right", "Front", "Back" };
		for (int i = 0; i < 6; i++)
		{
			ImGui::PushID(i);
			bool HasBit = point->DisplayedFaces & BIT(i);
			if (ImGui::Checkbox(names[i].data(), &HasBit))
			{
				point->DisplayedFaces ^= BIT(i);
			}
			ImGui::PopID();
		}
	}


}