#include "RealBatchRendererLayer.h"

#include <imgui.h>

#include <gtx/transform.hpp>
#include <iostream>
#include <glfw3.h>
#include <string>

#include "Core/Window.h"
#include "Renderables/Mesh.h"
#include "Player/ThirdPersonCamera.h"



RealBatchRendererLayer::RealBatchRendererLayer()
{

}


RealBatchRendererLayer::~RealBatchRendererLayer()
{

}


void RealBatchRendererLayer::OnAttach()
{
	//m_Renderer.OnAttach();
	//Renderer::Get()->OnAttach();

	m_Sprite = new Sprite(glm::vec2(0.0f, 0.0f), glm::vec2(0.75f, 0.5f));
	m_Sprite2 = new Sprite(glm::vec2(0.5f, 0.3f), glm::vec2(1.5f, 1.3f));

	const int LoadTexturesCount = 20;
	const int MaxTexturesCount = 200;
	const int TexturesCount = std::min(LoadTexturesCount, MaxTexturesCount);

	for (int i = 0; i < TexturesCount; i++)
	{
		//if (i == 50)
		//	__debugbreak();
		std::string filename = "res/Textures/RBR/" + std::to_string(i + 1) + ".png";
		m_Textures.push_back(std::make_shared<Texture2D>(filename));
		m_Materials.push_back(new Material(m_Textures[i], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		m_Sprites.push_back(new Sprite(glm::vec2((i / 16) / 16.0f, (i % 16) / 16.0f) - glm::vec2(1.0f / 16.0f * 4.0f), 
									   glm::vec2(1.0f / 16.0f) - glm::vec2(1.0f / 16.0f * 0.1f), m_Materials[i]));
		//m_Sprites.push_back(new Sprite(glm::vec2((i / 16) / 16.0f, (i % 16) / 16.0f) - glm::vec2(1.0f / 16.0f * 4.0f), 
		//							   glm::vec2(1.0f / 16.0f) - glm::vec2(1.0f / 16.0f * 0.1f), m_Materials[i]));
		//m_Sprites.push_back(new Sprite(glm::vec2((i / 16) / 16.0f, (i % 16) / 16.0f) - glm::vec2(1.0f / 16.0f * 4.0f), 
		//							   glm::vec2(1.0f / 16.0f) - glm::vec2(1.0f / 16.0f * 0.1f), m_Materials[i]));
		//m_Sprites.push_back(new Sprite(glm::vec2((i / 16) / 16.0f, (i % 16) / 16.0f) - glm::vec2(1.0f / 16.0f * 4.0f), 
		//							   glm::vec2(1.0f / 16.0f) - glm::vec2(1.0f / 16.0f * 0.1f), m_Materials[i]));
	}
	
	m_Sprites.push_back(new Sprite(glm::vec2((200 / 16) / 16.0f, (200 % 16) / 16.0f) - glm::vec2(1.0f / 16.0f * 4.0f), 
									   glm::vec2(1.0f / 16.0f) - glm::vec2(1.0f / 16.0f * 0.1f), m_Materials[199 % TexturesCount]));	
	m_Sprites.push_back(new Sprite(glm::vec2((201 / 16) / 16.0f, (201 % 16) / 16.0f) - glm::vec2(1.0f / 16.0f * 4.0f), 
									   glm::vec2(1.0f / 16.0f) - glm::vec2(1.0f / 16.0f * 0.1f), m_Materials[199 % TexturesCount]));


	m_BlendMaterials.push_back(new Material(nullptr, glm::vec4(1.0f, 0.0f, 0.0f, 0.5f)));

	m_BlendSprites.push_back(new Sprite(glm::vec2(0.0f, 0.0f), glm::vec2(2.0f / 16.0f, 2.0f / 16.0f), m_BlendMaterials.back()));
	//m_BlendSprites.back().
	m_BlendSprites.push_back(new Sprite(glm::vec2(0.0f, 0.0f), glm::vec2(2.0f / 16.0f, 2.0f / 16.0f), m_BlendMaterials.back()));
	m_BlendSprites.push_back(new Sprite(glm::vec2(0.0f, 0.0f), glm::vec2(2.0f / 16.0f, 2.0f / 16.0f), m_BlendMaterials.back()));


	//m_Camera = Camera(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f));
	//m_Projection = Projection(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f));
	m_Projection = Projection(glm::perspective(80.0f, 16.0f / 9.0f, 0.01f, 100.0f));
	//uint32_t m_VAO_test1;
	//uint32_t m_VBO_Vertex_test1;
	//
	//GLCall(glGenVertexArrays(1, &m_VAO_test1));
	//GLCall(glGenBuffers(1, &m_VBO_Vertex_test1));
	//	
	//GLCall(glBindVertexArray(m_VAO_test1));
	//
	//GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Vertex_test1));
	//GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW));
	//GLCall(glEnableVertexAttribArray(0));
	//GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0));
	//
	//uint32_t m_VAO_test2;
	//uint32_t m_VBO_Vertex_test2;
	//
	//GLCall(glGenVertexArrays(1, &m_VAO_test2));
	//GLCall(glGenBuffers(1, &m_VBO_Vertex_test2));
	//	
	//GLCall(glBindVertexArray(m_VAO_test2));
	//
	//GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Vertex_test2));
	//GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW));
	//GLCall(glEnableVertexAttribArray(0));
	//GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0));
	//
	//
	//glBindVertexArray(0);
	//
	//
	//glm::vec3* PositionsBuffer1;
	//glm::vec3* PositionsBuffer2;
	//GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Vertex_test1));
	//GLCall(PositionsBuffer1 = (glm::vec3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	//
	//
	//GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Vertex_test2));
	//GLCall(PositionsBuffer2 = (glm::vec3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	//
	//PositionsBuffer1[0] = glm::vec3(1.0f);
	//PositionsBuffer2[0] = glm::vec3(1.0f);


	//m_Model = new Model("./res/Models/BoneAnim5.fbx");
	m_Model = new Model("./res/Models/boblampclean.md5mesh");
}


void RealBatchRendererLayer::OnDetach()
{
	delete m_Sprite;
	delete m_Sprite2;
	delete m_Model;
	
	//m_Renderer.OnDetach();

	for (auto& material : m_Materials)
		delete material;

	for (auto& sprite : m_Sprites)
		delete sprite;	
	
	
	for (auto& material : m_BlendMaterials)
		delete material;

	for (auto& sprite : m_BlendSprites)
		delete sprite;	
}



void RealBatchRendererLayer::OnUpdate()
{
	Player::Get()->OnUpdate();
	


	Renderer::Get()->BeginScene(Player::Get()->GetCamera(), m_Projection, Lights::Get());
	//m_Renderer.BeginScene(m_Camera, m_Projection, m_Lights);

	for (int i = 0; i < m_Sprites.size(); i++)
		m_Sprites[i]->Submit(Renderer::Get());


	//m_Renderer.Submit(m_TestUnit);
	m_Chunk.Submit(Renderer::Get());

	m_Sprite->Submit(Renderer::Get());
	m_Sprite2->Submit(Renderer::Get());
	m_Model->Submit(Renderer::Get());

	for (auto& sprite : m_BlendSprites)
	{
		sprite->Submit(Renderer::Get());
	}

	GLCall(glEnable(GL_BLEND)); 
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_CULL_FACE));
	Renderer::Get()->OnUpdate();
	GLCall(glDisable(GL_CULL_FACE));
	GLCall(glDisable(GL_DEPTH_TEST));
}


void RealBatchRendererLayer::OnImGuiRender()
{
	if (ImGui::Begin("RealBatchRenderer"))
	{
		ImGui::Text("Sprite 1");
		m_Sprite->ImGuiDraw();
		ImGui::Separator();

		ImGui::Text("Sprite 2");
		m_Sprite2->ImGuiDraw();
		ImGui::Separator();
	}
	ImGui::End();

	if (ImGui::Begin("Blend"))
	{
		for (auto& sprite : m_BlendSprites)
		{
			ImGui::PushID(sprite);
			sprite->ImGuiDraw();
			ImGui::Separator();
			ImGui::PopID();
		}
	}
	ImGui::End();

	//if (ImGui::Begin("Lights2"))
	//{
	//	ImGui::DragFloat2("Angle", &m_Lights.Angle.x, 0.1f);
	//	ImGui::ColorEdit3("Ambient", &m_Lights.Ambient.x);
	//	ImGui::ColorEdit3("Diffuse", &m_Lights.Diffuse.x);
	//	ImGui::ColorEdit3("Specular", &m_Lights.Specular.x);
	//
	//}
	//ImGui::End();
}
