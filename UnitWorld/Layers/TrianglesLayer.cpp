#include "TrianglesLayer.h"

#include <imgui.h>

#include <gtx/transform.hpp>
#include <iostream>
#include <glfw3.h>
#include <string>

#include "Core/Window.h"
#include "Renderables/Mesh.h"
#include "Player/ThirdPersonCamera.h"

#define GLCall(x) x

TrianglesLayer::TrianglesLayer() 
{

}


TrianglesLayer::~TrianglesLayer()
{
}


void TrianglesLayer::OnAttach()
{
	GLfloat vertices[] = {
		-0.5f, -0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		 0.0f,  0.5f,  0.0f,

		-1.0f, -1.0f,  0.0f,
		-0.5f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f };
		
	GLfloat texCoord[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.0f,  0.5f,

		-1.0f, -1.0f,
		-0.5f,  0.0f,
		-1.0f,  0.0f};

	GLshort vertMaterial[] = {
		0,
		0,
		0,
		  
		1,
		1,
		1
	};
	GLshort vertTransform[] = {
		0,
		0,
		0,
		  
		1,
		1,
		1
	};

	GLCall(glGenVertexArrays(1, &m_VAO));
	GLCall(glGenBuffers(1, &m_VBO_Vertex));
	GLCall(glGenBuffers(1, &m_VBO_TexCoord));
	GLCall(glGenBuffers(1, &m_VBO_MaterialId));
	GLCall(glGenBuffers(1, &m_VBO_TransformId));
		
	GLCall(glBindVertexArray(m_VAO));
	
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Vertex));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO_TexCoord));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO_MaterialId));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertMaterial), NULL, GL_STATIC_DRAW));
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribIPointer(2, 1, GL_SHORT, 0, 0));
	
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO_TransformId));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertTransform), NULL, GL_STATIC_DRAW));
	GLCall(glEnableVertexAttribArray(3));
	GLCall(glVertexAttribIPointer(3, 1, GL_SHORT, 0, 0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

	//GLushort indices[6] =
	//{
	//	0, 1, 2, 0, 1, 3
	//};
	//
	//
	//GLCall(glGenBuffers(1, &m_IBO));
	//GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	//GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), indices, GL_STATIC_DRAW));
	//GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


		
	GLCall(glBindVertexArray(0));
	

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_MaterialId);
	short* matBuffer = (short*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_TransformId);
	short* transBuffer = (short*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	for (int i = 0; i < 6; i++)
		transBuffer[i] = vertTransform[i];

	for (int i = 0; i < 6; i++)
		matBuffer[i] = vertMaterial[i];


	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_MaterialId);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_TransformId);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	
	m_Shader = new Shader({ "./res/Shaders/2D/TestShader.vert", "./res/Shaders/2D/TestShader.frag" });
	m_Shader->Enable();
	GLint texIDs[]{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
	
	m_Shader->SetUniform1iv("uTextures", texIDs, 31);

	glGenBuffers(1, &m_MatrixBuffer);
	glBindBuffer(GL_TEXTURE_BUFFER, m_MatrixBuffer);
	glBufferData(GL_TEXTURE_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STATIC_DRAW);

	glGenTextures(1, &m_TextureBuffer);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, m_TextureBuffer);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_MatrixBuffer);



	m_TestTexture = new Texture2D("./res/Textures/Test.png");


	GLCall(glGenTextures(1, &m_TextureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
		
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	unsigned char pixels[4 * 4] = { 0xff, 0x00, 0xff, 0xff,
									0x14, 0x14, 0x14, 0xff,
									0x14, 0x14, 0x14, 0xff,
									0xff, 0x00, 0xff, 0xff, };


	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));


#if 0
	m_Renderer.OnAttach();
	m_Sprite = new Sprite(glm::vec2(0.0f, 0.0f), glm::vec2(0.75f, 0.5f));
	m_Sprite2 = new Sprite(glm::vec2(0.5f, 0.3f), glm::vec2(1.5f, 1.3f));

	const int TexturesCount = 200;

	for (int i = 0; i < TexturesCount; i++)
	{
		//if (i == 50)
		//	__debugbreak();
		std::string filename = "res/Textures/RBR/" + std::to_string(i + 1) + ".png";
		m_Textures.push_back(new Texture2D(filename));
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
									   glm::vec2(1.0f / 16.0f) - glm::vec2(1.0f / 16.0f * 0.1f), m_Materials[199]));	
	m_Sprites.push_back(new Sprite(glm::vec2((201 / 16) / 16.0f, (201 % 16) / 16.0f) - glm::vec2(1.0f / 16.0f * 4.0f), 
									   glm::vec2(1.0f / 16.0f) - glm::vec2(1.0f / 16.0f * 0.1f), m_Materials[199]));	


	m_BlendMaterials.push_back(new Material(nullptr, glm::vec4(1.0f, 0.0f, 0.0f, 0.5f)));

	m_BlendSprites.push_back(new Sprite(glm::vec2(0.0f, 0.0f), glm::vec2(2.0f / 16.0f, 2.0f / 16.0f), m_BlendMaterials.back()));
	//m_BlendSprites.back().
	m_BlendSprites.push_back(new Sprite(glm::vec2(0.0f, 0.0f), glm::vec2(2.0f / 16.0f, 2.0f / 16.0f), m_BlendMaterials.back()));
	m_BlendSprites.push_back(new Sprite(glm::vec2(0.0f, 0.0f), glm::vec2(2.0f / 16.0f, 2.0f / 16.0f), m_BlendMaterials.back()));


	//m_Camera = Camera(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f));
	//m_Projection = Projection(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f));
	m_Projection = Projection(glm::perspective(80.0f, 16.0f / 9.0f, 0.01f, 100.0f));
	m_Player = new Player(new ThirdPersonCamera);
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

#endif
}


void TrianglesLayer::OnDetach()
{
	
	delete m_TestTexture;
	delete m_Shader;
#if 0
	delete m_Sprite;
	delete m_Sprite2;
	delete m_Model;
	m_Renderer.OnDetach();

	for (auto& texture : m_Textures)
		delete texture;

	for (auto& material : m_Materials)
		delete material;

	for (auto& sprite : m_Sprites)
		delete sprite;	
	
	
	for (auto& material : m_BlendMaterials)
		delete material;

	for (auto& sprite : m_BlendSprites)
		delete sprite;	
#endif
}


void TrianglesLayer::OnUpdate() 
{
	
	GLCall(glBindBuffer(GL_TEXTURE_BUFFER, m_MatrixBuffer));
	glm::mat4 * Buffer;
	GLCall(Buffer = (glm::mat4*)glMapBuffer(GL_TEXTURE_BUFFER, GL_WRITE_ONLY));
	Buffer++[0] = glm::translate(m_TriangleTransforms[0]);
	Buffer[0] = glm::translate(m_TriangleTransforms[1]);
	GLCall(glUnmapBuffer(GL_TEXTURE_BUFFER));
	

	m_Shader->Enable();
	m_Shader->SetUniform4f("colors[0]", m_TriangleColors[0]);
	m_Shader->SetUniform4f("colors[1]", m_TriangleColors[1]);

	m_Shader->SetUniformMat4("transforms[0]", glm::translate(m_TriangleTransforms[0]));
	m_Shader->SetUniformMat4("transforms[1]", glm::translate(m_TriangleTransforms[1]));
	GLCall(glBindVertexArray(m_VAO));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, m_TextureBuffer);
	//glBindTextureUnit(0, m_TextureBuffer);

	//glBindTextureUnit(1, m_TextureID);

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, m_TextureID);
	m_TestTexture->Bind(1);
	//m_Textures[50]->Bind(1);


	//GLsizei MultiCount[] = { 3, 3 };
	//GLint MultiFirst[] = { 0, 3 };

	//glMultiDrawArrays(GL_TRIANGLES, &MultiFirst[0], &MultiCount[0], 2);
	//GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL));

	
	std::vector<uint32_t> ind1 = { 0, 1, 2 };
	std::vector<uint32_t> ind2 = { 3, 4, 5 };

	std::vector<std::vector<uint32_t>> indicesArray;
	indicesArray.push_back(ind1);

	//GLvoid *MultiIndices[2] = { &ind1[0], &ind2[0] };

	std::vector<int> m_Counts = { 3, 3 };
	std::vector<GLvoid*> indices;
	indices.push_back(&indicesArray.back()[0]);
	//indices.push_back(&ind1[0]);
	indices.push_back(&ind2[0]);

	glMultiDrawElements(GL_TRIANGLES, &m_Counts[0], GL_UNSIGNED_INT, &indices[0], 2);
	



	GLCall(glBindVertexArray(0));


#if 0
	m_Player->OnUpdate();
	m_Renderer.BeginScene(m_Player->GetCamera(), m_Projection, m_Lights);
	//m_Renderer.BeginScene(m_Camera, m_Projection, m_Lights);

	for (int i = 0; i < m_Sprites.size(); i++)
		m_Sprites[i]->Submit(&m_Renderer);


	//m_Renderer.Submit(m_TestUnit);
	m_Chunk.Submit(&m_Renderer);

	m_Sprite->Submit(&m_Renderer);
	m_Sprite2->Submit(&m_Renderer);
	m_Model->Submit(&m_Renderer);

	for (auto& sprite : m_BlendSprites)
	{
		sprite->Submit(&m_Renderer);
	}

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	if (ImGui::IsKeyPressed(GLFW_KEY_E))
		m_EditCamera = !m_EditCamera;

	if (m_EditCamera)
	{
		m_Player->GetCamera()->addRotation(glm::vec2(io.MouseDelta.x / 4.0f, -io.MouseDelta.y / 4.0f));
		//m_Camera.addRotation(glm::vec2(io.MouseDelta.x / 4.0f, -io.MouseDelta.y / 4.0f));
	}

	GLCall(glEnable(GL_BLEND)); 
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_CULL_FACE));
	m_Renderer.OnUpdate();
	GLCall(glDisable(GL_CULL_FACE));
	GLCall(glDisable(GL_DEPTH_TEST));
#endif

}


void TrianglesLayer::OnImGuiRender() 
{

	if (ImGui::Begin("Triangle"))
	{
		ImGui::DragFloat2("First Transform", &m_TriangleTransforms[0].r, 0.01f);
		ImGui::ColorEdit4("First Color", &m_TriangleColors[0].r); 
		ImGui::Separator();
		ImGui::DragFloat2("Second Transform", &m_TriangleTransforms[1].r, 0.01f);
		ImGui::ColorEdit4("Second Color", &m_TriangleColors[1].r); 
	}
	ImGui::End();

#if 0
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


	if (ImGui::Begin("Camera"))
	{
		Camera &m_Camera = *m_Player->GetCamera();
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
		ImGui::DragFloat2("Angle", &m_Lights.Angle.x, 0.1f);
		ImGui::ColorEdit3("Ambient", &m_Lights.Ambient.x);
		ImGui::ColorEdit3("Diffuse", &m_Lights.Diffuse.x);
		ImGui::ColorEdit3("Specular", &m_Lights.Specular.x);

	}
	ImGui::End();
#endif
}

