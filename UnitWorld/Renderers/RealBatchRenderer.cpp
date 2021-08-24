#include "RealBatchRenderer.h"

#include "Renderables/Renderable.h"
#include "Debug/Instrumentor.h"
#include "GLCall.h"
#include "Player/Camera.h"
#include "Debug/ConsoleLog.h"
#include "Core/Timer.h"

#include <iostream>
#include <string>
#include <cassert>
#include <algorithm>
#include <future>
#include <gtx/transform.hpp>
#include <glfw3.h>
#include <sstream>

#if TEST_MT
ThreadPool RealBatchRenderer::s_ThreadPool = ThreadPool(10);
#endif

//////////////////////////////////////////////////////////////////////////////
////////// SubBuffer /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void SubBuffer::ClearBuffers()
{
	m_IndexBuffer.Reset();
	m_Texures.clear();
	m_Materials.clear();
	m_MaterialsInfo.clear();
}


void SubBuffer::BindVAO()
{
	glBindVertexArray(m_VAO);
}


void SubBuffer::BindIBO()
{
	m_IndexBuffer.Bind();
}


void SubBuffer::BindTransformTexture(int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_BUFFER, m_TransfortionTexture);
}


void SubBuffer::SetMaterials(Shader* shader)
{
	for (int i = 0; i < m_Materials.size(); i++)
	{
		shader->SetMaterial(i, m_Materials[i], &m_MaterialsInfo[i]);
	}
}


void SubBuffer::SetTextures()
{
	for (int i = 0; i < m_Texures.size(); i++)
	{
		//glActiveTexture(GL_TEXTURE0 + i + 1);
		//glBindTexture(GL_TEXTURE_2D, m_Texures[i]);
		m_TexturePointers[i]->Bind(i + RealBatchRenderer::s_NotSimpleTextures);
	}
}






//////////////////////////////////////////////////////////////////////////////
////////// IndicesBuffer /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

IndicesBuffer::IndicesBuffer()
{
	glGenBuffers(1, &m_BufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_VERTICES * sizeof(uint32_t) * 6, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Reset();
}


IndicesBuffer::~IndicesBuffer()
{
	glDeleteBuffers(1, &m_BufferID);
}


void IndicesBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
}

void IndicesBuffer::StartScene()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
#if !USE_SUB_DATA
	m_Buffer = (uint32_t*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
#endif
}


void IndicesBuffer::EndScene()
{
#if !USE_SUB_DATA
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
#endif
}


void IndicesBuffer::Add(const std::vector<uint32_t>& indices, uint32_t VerticesCount)
{
#if USE_SUB_DATA 
	std::vector<uint32_t> CorectIndices = indices;
	for (int i = 0; i < CorectIndices.size(); i++)
	{
		CorectIndices[i] += m_MaxIndex;
	}
	//std::for_each(CorectIndices.begin(), CorectIndices.end(), [&](uint32_t v) { v += m_MaxIndex; });
	
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_Size * sizeof(uint32_t), CorectIndices.size() * sizeof(uint32_t), &CorectIndices[0]);

#else
	for (int i = 0; i < indices.size(); i++)
	{
		m_Buffer[0] = indices[i] + m_MaxIndex;
		m_Buffer++;
	}
#endif

	m_Size += indices.size();
	m_MaxIndex += VerticesCount;
	m_ObjectsCount++;
}


void IndicesBuffer::Reset()
{
	m_ObjectsCount = 0;
	m_MaxIndex = 0;
	m_Size = 0;
}






//////////////////////////////////////////////////////////////////////////////
////////// SimpleSubBuffer ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

SimpleSubBuffer::SimpleSubBuffer()
	: m_PositionsBuffer(nullptr), m_UVsBuffer(nullptr), m_NormalsBuffer(nullptr), m_TangentsBuffer(nullptr)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO_Positions);
	glGenBuffers(1, &m_VBO_UVs);
	glGenBuffers(1, &m_VBO_Normals);
	glGenBuffers(1, &m_VBO_Tangents);
	
	glGenBuffers(1, &m_VBO_MaterialId);
	glGenBuffers(1, &m_VBO_TransformId);


	glBindVertexArray(m_VAO);

	unsigned int id = 0;

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_VERTICES, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(id);
	glVertexAttribPointer(id++, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_UVs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * MAX_VERTICES, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(id);
	glVertexAttribPointer(id++, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_VERTICES, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(id);
	glVertexAttribPointer(id++, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Tangents);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_VERTICES, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(id);
	glVertexAttribPointer(id++, 3, GL_FLOAT, GL_FALSE, 0, 0);


	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_MaterialId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(short) * MAX_VERTICES, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(id);
	glVertexAttribIPointer(id++, 1, GL_SHORT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_TransformId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(short) * MAX_VERTICES, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(id);
	glVertexAttribIPointer(id++, 1, GL_SHORT, 0, 0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glGenBuffers(1, &m_MatrixTextureBuffer);
	glBindBuffer(GL_TEXTURE_BUFFER, m_MatrixTextureBuffer);
	glBufferData(GL_TEXTURE_BUFFER, sizeof(glm::mat4) * MAX_VERTICES, NULL, GL_DYNAMIC_DRAW);

	glGenTextures(1, &m_TransfortionTexture);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, m_TransfortionTexture);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_MatrixTextureBuffer);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);
}


SimpleSubBuffer::~SimpleSubBuffer()
{

	glDeleteBuffers(1, &m_VBO_Positions);
	glDeleteBuffers(1, &m_VBO_UVs);
	glDeleteBuffers(1, &m_VBO_Normals);
	glDeleteBuffers(1, &m_VBO_Tangents);
	glDeleteBuffers(1, &m_VBO_MaterialId);
	glDeleteBuffers(1, &m_VBO_TransformId);

	glDeleteBuffers(1, &m_MatrixTextureBuffer);

	glDeleteTextures(1, &m_TransfortionTexture);

	glDeleteVertexArrays(1, &m_VAO);
}


//static TimerStartEnd AddTimer;
void SimpleSubBuffer::StartScene()
{
	//AddTimer.Start();

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Positions));
	GLCall(m_PositionsBuffer = (glm::vec3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO_UVs));
	GLCall(m_UVsBuffer = (glm::vec2*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Normals));
	GLCall(m_NormalsBuffer = (glm::vec3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Tangents));
	GLCall(m_TangentsBuffer = (glm::vec3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_MaterialId);
	m_MaterialsBuffer = (short*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_TransformId);
	short* transBuffer = (short*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	m_TransformsBuffer = transBuffer;
	
	glBindBuffer(GL_TEXTURE_BUFFER, m_MatrixTextureBuffer);
	m_MatrixBuffer = (glm::mat4*)glMapBuffer(GL_TEXTURE_BUFFER, GL_WRITE_ONLY);

	m_IndexBuffer.StartScene();

	
	m_FMID.clear();				// FIX IT!!!
	m_Transforms.clear();
	m_Objects.clear();
}


void SimpleSubBuffer::EndScene()
{

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Positions); 
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_UVs);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Normals);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Tangents);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_MaterialId);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_TransformId);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	
	glBindBuffer(GL_TEXTURE_BUFFER, m_MatrixTextureBuffer); 
	glUnmapBuffer(GL_TEXTURE_BUFFER);



	m_IndexBuffer.EndScene();
	//AddTimer.End();
}


bool SimpleSubBuffer::AddObject(Renderable *renderable, const glm::mat4& transform)
{
	short FoundedMaterailId = m_Materials.size();

	for (short i = 0; i < m_Materials.size(); i++)
	{
		if (m_Materials[i] == renderable->GetMaterial())
		{
			FoundedMaterailId = i;
			break;
		}
	}

	if (FoundedMaterailId == m_Materials.size())
	{
		if (renderable->GetMaterial()->Diffuse)
		{
			short FoundedTextureSlot = m_Texures.size();
			for (short i = 0; i < m_Texures.size(); i++)
			{
				if (m_Texures[i] == renderable->GetMaterial()->Diffuse->GetId())
				{
					FoundedTextureSlot = i;
					m_Materials.push_back(renderable->GetMaterial());
					m_MaterialsInfo.push_back(MaterialInfo());
					m_MaterialsInfo[FoundedMaterailId] = MaterialInfo{ i };
					break;
				}
			}
			if (FoundedTextureSlot == m_Texures.size())
			{
				if (FoundedTextureSlot >= RealBatchRenderer::s_MaxTexturesToDraw)
					return false;
				m_Materials.push_back(renderable->GetMaterial());
				m_MaterialsInfo.push_back(MaterialInfo());
				m_Texures.push_back(renderable->GetMaterial()->Diffuse->GetId());
				m_TexturePointers.push_back(renderable->GetMaterial()->Diffuse);
				m_MaterialsInfo[FoundedMaterailId] = MaterialInfo{ FoundedTextureSlot };
			}
		}
		else
		{
			m_Materials.push_back(renderable->GetMaterial());
			m_MaterialsInfo.push_back(MaterialInfo{ -1 });
		}

	}

	m_FMID.emplace_back(FoundedMaterailId);
	m_Transforms.emplace_back(transform);
	m_Objects.emplace_back(renderable);

#if !TEST_MT
	memcpy(&m_PositionsBuffer[0], &renderable->GetPositions()[0], renderable->GetPositions().size() * sizeof(renderable->GetPositions()[0]));
	m_PositionsBuffer += renderable->GetPositions().size();

	memcpy(&m_UVsBuffer[0], &renderable->GetUVs()[0], renderable->GetUVs().size() * sizeof(renderable->GetUVs()[0]));
	m_UVsBuffer += renderable->GetUVs().size();

	memcpy(&m_NormalsBuffer[0], &renderable->GetNormals()[0], renderable->GetNormals().size() * sizeof(renderable->GetNormals()[0]));
	m_NormalsBuffer += renderable->GetNormals().size();

	memcpy(&m_TangentsBuffer[0], &renderable->GetTangents()[0], renderable->GetTangents().size() * sizeof(renderable->GetTangents()[0]));
	m_TangentsBuffer += renderable->GetTangents().size();

	std::fill(m_MaterialsBuffer, m_MaterialsBuffer + renderable->GetPositions().size(), FoundedMaterailId);
	m_MaterialsBuffer += renderable->GetPositions().size();

	std::fill(m_TransformsBuffer, m_TransformsBuffer + renderable->GetPositions().size(), m_IndexBuffer.m_ObjectsCount);
	m_TransformsBuffer += renderable->GetPositions().size();

	memcpy(&m_MatrixBuffer[0], &transform, sizeof(transform));
	m_MatrixBuffer++;

	m_IndexBuffer.Add(renderable->GetIndices(), renderable->GetPositions().size());
#endif


	return true;

}


std::future<void> SimpleSubBuffer::UpdateBuffers()
{
#if TEST_MT
	auto t =  RealBatchRenderer::s_ThreadPool.Enqueue([=]() {

		auto t1 = RealBatchRenderer::s_ThreadPool.Enqueue([=]() {
			for (uint32_t i = 0; i < m_Objects.size(); i++)
			{
				memcpy(&m_PositionsBuffer[0], &m_Objects[i]->GetPositions()[0], m_Objects[i]->GetPositions().size() * sizeof(m_Objects[i]->GetPositions()[0]));
				m_PositionsBuffer += m_Objects[i]->GetPositions().size();
			}
		});

		auto t2 = RealBatchRenderer::s_ThreadPool.Enqueue([=]() {
			for (uint32_t i = 0; i < m_Objects.size(); i++)
			{
				memcpy(&m_UVsBuffer[0], &m_Objects[i]->GetUVs()[0], m_Objects[i]->GetUVs().size() * sizeof(m_Objects[i]->GetUVs()[0]));
				m_UVsBuffer += m_Objects[i]->GetUVs().size();
			}
		});

		auto t3 = RealBatchRenderer::s_ThreadPool.Enqueue([=]() {
			for (uint32_t i = 0; i < m_Objects.size(); i++)
			{
				memcpy(&m_NormalsBuffer[0], &m_Objects[i]->GetNormals()[0], m_Objects[i]->GetNormals().size() * sizeof(m_Objects[i]->GetNormals()[0]));
				m_NormalsBuffer += m_Objects[i]->GetNormals().size();
			}
		});

		auto t4 = RealBatchRenderer::s_ThreadPool.Enqueue([=]() {
			for (uint32_t i = 0; i < m_Objects.size(); i++)
			{
				memcpy(&m_TangentsBuffer[0], &m_Objects[i]->GetTangents()[0], m_Objects[i]->GetTangents().size() * sizeof(m_Objects[i]->GetTangents()[0]));
				m_TangentsBuffer += m_Objects[i]->GetTangents().size();
			}
		});

		auto t5 = RealBatchRenderer::s_ThreadPool.Enqueue([=]() {
			for (uint32_t i = 0; i < m_Objects.size(); i++)
			{
				std::fill(m_MaterialsBuffer, m_MaterialsBuffer + m_Objects[i]->GetPositions().size(), m_FMID[i]);
				m_MaterialsBuffer += m_Objects[i]->GetPositions().size();
			}
		});


		auto t6 = RealBatchRenderer::s_ThreadPool.Enqueue([=]() {
			for (uint32_t i = 0; i < m_Objects.size(); i++)
			{
				memcpy(&m_MatrixBuffer[0], &m_Transforms[i], sizeof(m_Transforms[0]));
				m_MatrixBuffer++;
			}
		});

		auto t7 = RealBatchRenderer::s_ThreadPool.Enqueue([=]() {
			for (uint32_t i = 0; i < m_Objects.size(); i++)
			{
				std::fill(m_TransformsBuffer, m_TransformsBuffer + m_Objects[i]->GetPositions().size(), m_IndexBuffer.m_ObjectsCount);
				m_TransformsBuffer += m_Objects[i]->GetPositions().size();
		
				m_IndexBuffer.Add(m_Objects[i]->GetIndices(), m_Objects[i]->GetPositions().size());
			}
		});


		t1.get();
		t2.get();
		t3.get();
		t4.get();
		t5.get();
		t6.get();
		t7.get();
	});
	return t;
#else
	return std::future<void>();
#endif
}



//////////////////////////////////////////////////////////////////////////////
////////// SimpleSubBuffer ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

SimpleBufferManager::SimpleBufferManager()
{
	m_Shader = new Shader(ShaderPaths("./res/Shaders/RealBatchRenderer/TestShader.vert", "./res/Shaders/RealBatchRenderer/TestShader.frag"));

	m_Shader->Enable();
	
	m_Shader->SetUniform1i("u_TransformTexture", 0);


	//const size_t IDsCount = 32;
	//const size_t PackCount = 6;
	//const size_t MaxTextures = 192;
	//const size_t NoneSimpleTextures = 1;
	
	int IDsCount;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &IDsCount);
	int MaxTextures;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &MaxTextures);
	size_t PackCount = MaxTextures / IDsCount;

	for (int i = 1; i <= PackCount; i++)
	{
		std::vector<GLint> texIDs;
		for (int j = (i - 1) * IDsCount + 1; j <= glm::min(IDsCount * i, MaxTextures - RealBatchRenderer::s_NotSimpleTextures); j++)
		{
			texIDs.push_back(j);
		}
		std::string name = "u_Textures" + std::to_string(i);
		m_Shader->SetUniform1iv(name, texIDs.data(), IDsCount);
	}
	AddBuffer();
}


SimpleBufferManager::~SimpleBufferManager()
{
	delete m_Shader;
	for (auto& buffer : m_Buffers)
	{
		delete buffer;
	}
}


void SimpleBufferManager::AddObject(Renderable* renderable, const glm::mat4& transform)
{
	for (auto& buffer : m_Buffers)
	{
		if (buffer->AddObject(renderable, transform))
		{
			return;
		}
	}

	LOGI("Adding New Buffer!");
	//EndScene();
	//std::lock_guard lock(s_Mtx);
	//s_Mtx.lock();
	AddBuffer();
	//StartScene();
	m_Buffers.back()->StartScene();
	//s_Mtx.unlock();
	m_Buffers.back()->AddObject(renderable, transform);

	LOGI("Added In New Buffer!");
}


void SimpleBufferManager::AddBuffer()
{
	m_Buffers.push_back(new SimpleSubBuffer);
}


void SimpleBufferManager::DrawBuffer(const Camera *camera, const Projection &projection, Lights* lights)
{
	EnableShader(camera, projection, lights);
	for (uint32_t i = 0; i < GetBufferSize(); i++)
	{
		BindBuffer(i);
		if (GetObjectsCount(i))
			GLCall(glDrawElements(GL_TRIANGLES, GetIndicesCount(i), GL_UNSIGNED_INT, NULL));
	}
}



void SimpleBufferManager::StartScene()
{
	for (auto& buffer : m_Buffers)
	{
		buffer->StartScene();
	}
}


void SimpleBufferManager::EndScene()
{
	for (auto& buffer : m_Buffers)
	{
		buffer->EndScene();
	}
}


void SimpleBufferManager::ClearBuffers()
{
	for (auto& buffer : m_Buffers)
	{
		buffer->ClearBuffers();
	}
}


void SimpleBufferManager::EnableShader(const Camera *camera, const Projection &projection, Lights* lights)
{
	m_Shader->Enable();
	m_Shader->SetUniformMat4("u_ProjectionMatrix", projection.GetTransform());
	//glm::vec3 from = glm::rotate(float(glfwGetTime()) / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	//m_Shader->SetUniformMat4("uViewMatrix", glm::lookAt(from, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	m_Shader->SetUniformMat4("u_ViewMatrix", camera->GetMaxtrix());
	m_Shader->SetUniform3f("u_ViewPos", camera->getPosition());
	

	m_Shader->SetUniform3f("u_DirLight.direction", lights->GetDirection());
	m_Shader->SetUniform3f("u_DirLight.ambient", lights->GetAmbient());
	m_Shader->SetUniform3f("u_DirLight.diffuse", lights->GetDiffuse());
	m_Shader->SetUniform3f("u_DirLight.specular", lights->GetSpecular());

}


void SimpleBufferManager::BindBuffer(uint32_t id)
{
	m_Buffers[id]->BindVAO();
	m_Buffers[id]->BindIBO();
	m_Buffers[id]->BindTransformTexture();
	m_Buffers[id]->SetMaterials(m_Shader);
	m_Buffers[id]->SetTextures();
}

void SimpleBufferManager::UpdateBuffers()
{
	std::vector<std::future<void>> results;
	for (auto& buffer : m_Buffers)
	{
		results.emplace_back(buffer->UpdateBuffers());
	}

	for (auto& result : results)
	{
		result.get();
	}
}


//////////////////////////////////////////////////////////////////////////////
////////// RealBatchRenderer /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RealBatchRenderer::RealBatchRenderer()
	/*: m_Shader(nullptr)*/ { }


RealBatchRenderer::~RealBatchRenderer()
{

}


void RealBatchRenderer::OnAttach()
{
	m_SimpleBuffer = new SimpleBufferManager;
	
	m_TransformationStack.push_back(glm::mat4(1.0f));
	m_TransformationBack = &m_TransformationStack.back();


	int IDsCount;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &IDsCount);
	int MaxTextures;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &MaxTextures);
	size_t PackCount = MaxTextures / IDsCount;
	s_NotSimpleTextures = 1;

	s_MaxTexturesToDraw = MaxTextures - s_NotSimpleTextures;
}


void RealBatchRenderer::OnDetach()
{
	m_TransformationStack.clear();
	delete m_SimpleBuffer;
}

#define USE_MANAGER true

void RealBatchRenderer::OnUpdate()
{
	PROFILE_FUNCTION();

#if USE_MANAGER
	
#if TEST_MT
	m_SimpleBuffer->UpdateBuffers();
#endif

	m_SimpleBuffer->EndScene();
#else
	m_Objects.EndScene();
#endif
	Flush();
}

void RealBatchRenderer::PushTransform(glm::mat4 *transform, bool override)
{
	if (override)
		m_TransformationStack.push_back(*transform);
	else
		m_TransformationStack.push_back(*m_TransformationBack * (*transform));
	
	m_TransformationBack = &m_TransformationStack.back();
}

void RealBatchRenderer::PopTransform()
{
	if (m_TransformationStack.size() > 1)
		m_TransformationStack.pop_back();

	m_TransformationBack = &m_TransformationStack.back();
}

void RealBatchRenderer::BeginScene(Camera *camera, const Projection & projection, Lights* lights)
{
	PROFILE_FUNCTION();

	m_Camera = camera;
	m_Projection = projection;
	m_Lights = lights;

#if USE_MANAGER
	m_SimpleBuffer->ClearBuffers();
	m_SimpleBuffer->StartScene();
#else
	m_Objects.ClearBuffers();
	m_Objects.StartScene();
#endif
}


void RealBatchRenderer::Submit(Renderable* renderable)
{
#if USE_MANAGER
	
	m_SimpleBuffer->AddObject(renderable, *m_TransformationBack);
#else
	m_Objects.AddObject(renderable, *m_TransformationBack);
#endif
}


void RealBatchRenderer::Flush()
{
	PROFILE_FUNCTION();

#if USE_MANAGER
	
	m_SimpleBuffer->DrawBuffer(m_Camera, m_Projection, m_Lights);

#endif
}


