#include "BatchRendererLayer.h"

#include "Renderers/BatchRenderer.h"
#include "GLCall.h"
#include <imgui.h>
#include "Player/Player.h"
#include "Manager/Manager.h"

namespace LM
{

	BatchRendererLayer::BatchRendererLayer()
	{

	}

	BatchRendererLayer::~BatchRendererLayer()
	{

	}

	void BatchRendererLayer::OnAttach()
	{
		//BufferLayout layout{ { ShaderDataType::Float3, "Position" },
		//					 { ShaderDataType::Float2, "UV" },
		//					 { ShaderDataType::Float3, "Normals" },
		//					 { ShaderDataType::Float3, "Tangents" },
		//					 { ShaderDataType::Int,    "MaterialID" },
		//					 { ShaderDataType::Int,    "TransformID" } };
		//
		//float size = 40.0f;
		//
		//Vertex vertices[] = { { glm::vec3( 0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f), 1, 0 },
		//					   { glm::vec3(size, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f), 1, 0 },
		//					   { glm::vec3(size, size, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f), 1, 0 },
		//					   { glm::vec3(0.0f, size, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f), 1, 0 } };
		//
		//m_VBO = std::make_shared<VertexBuffer>(layout.GetStride() * 4);
		//m_VBO->SetLayout(layout);
		//m_VBO->SetData(vertices, sizeof(Vertex) * 4);
		//
		//m_Indieces = std::make_shared<IndexBuffer>(6);
		//
		//m_VAO = std::make_shared<VertexArray>();
		//m_VAO->AddVertexBuffer(m_VBO);
		//m_VAO->SetIndexBuffer(m_Indieces);
		//
		//m_Position = glm::vec3(0.0f);
		//m_Transform = glm::translate(m_Position);
		//m_TextureBuffer = std::make_shared<TextureBuffer>(&m_Transform[0], sizeof(m_Transform));

		m_Renderer = new BatchRenderer;
		m_Renderer->OnAttach();

		Managers::Texture.Add("Calcium", std::make_shared<Texture2D>("./res/Textures/S5aTkZoBkBQ.jpg"));
		//m_Texture = std::make_shared<Texture2D>("./res/Textures/38UNp4Gt-d8.jpg");
		m_Material = std::make_shared<Material>(Material(Managers::Texture.Get("Calcium"), glm::vec4(1.0f)));

		m_Sprite = std::make_shared<Sprite>(glm::vec2(0.0f), glm::vec2(5.0f, 5.0f), &(*m_Material));
		m_Renderer->Submit(m_Sprite);
		//m_Sprite->Submit(m_Renderer);
	}

	void BatchRendererLayer::OnDetach()
	{
		m_Renderer->OnDetach();
	}

	void BatchRendererLayer::OnUpdate()
	{
		//uint32_t indices[] = { 0u, 1u, 2u, 0u, 2u, 3u };
		//m_Indieces->SetIndices(indices, 6);
		//m_Transform = glm::translate(m_Position);
		//m_TextureBuffer->SetData(&m_Transform[0], sizeof(m_Transform));
		//m_TextureBuffer->Bind(0);
		//m_VAO->Bind();
		//m_Indieces->Bind();
		//GLCall(glDrawElements(GL_TRIANGLES, m_Indieces->GetCount(), GL_UNSIGNED_INT, NULL));
		
		
		
		m_Renderer->BeginScene(Player::Get()->GetCamera(), m_Projection, Lights::Get());


		m_Renderer->OnUpdate();

	}

	void BatchRendererLayer::OnImGuiRender()
	{
		if (ImGui::Begin("BatchRendererLayer"))
		{
			ImGui::DragFloat3("Position", &m_Position[0]);
		}
		ImGui::End();
	}


}