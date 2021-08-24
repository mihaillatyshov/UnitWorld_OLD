#include "BatchRenderer.h"

#include "Renderables/Renderable.h"
#include <string>

namespace LM
{
	//////////////////////////////////////////////////////////////////////////////
	////////// SimpleObjectManager ///////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	SimpleObjectManager::SimpleObjectManager(BatchRenderer* renderer)
		: m_Renderer(renderer)
	{
		m_Shader = std::make_unique<Shader>(ShaderPaths("./res/Shaders/BatchRenderer/TestShader.vert", "./res/Shaders/BatchRenderer/TestShader.frag"));

		m_Shader->Enable();
		m_Shader->SetUniform1i("u_TransformTexture", 0);

		int IDsCount;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &IDsCount);
		int MaxTextures;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &MaxTextures);
		//int IDsCount = 32;
		//int MaxTextures = 192;
		size_t PackCount = MaxTextures / IDsCount;
		m_NotSimpleTextures = 1;

		m_MaxTexturesToDraw = MaxTextures - m_NotSimpleTextures;
	
		for (int i = 1; i <= PackCount; i++)
		{
			std::vector<GLint> texIDs;
			for (size_t j = (i - 1) * IDsCount + m_NotSimpleTextures; j <= glm::min(IDsCount * i, MaxTextures - m_NotSimpleTextures); j++)
			{
				texIDs.push_back(j);
			}
			std::string name = "u_Textures" + std::to_string(i);
			m_Shader->SetUniform1iv(name, texIDs.data(), texIDs.size());
		}

		BufferLayout layout{ { ShaderDataType::Float3, "Position" },
							 { ShaderDataType::Float2, "UV" },
							 { ShaderDataType::Float3, "Normals" },
							 { ShaderDataType::Float3, "Tangents" },
							 { ShaderDataType::Int,    "MaterialID" },
							 { ShaderDataType::Int,    "TransformID" } };

		m_VBO = std::make_shared<VertexBuffer>(layout.GetStride() * BatchRenderer::m_MaxVertices);
		m_VBO->SetLayout(layout);

		m_IBO = std::make_shared<IndexBuffer>(BatchRenderer::m_MaxIndices);

		m_VAO = std::make_shared<VertexArray>();
		m_VAO->AddVertexBuffer(m_VBO);
		m_VAO->SetIndexBuffer(m_IBO);

		m_TransformTexture = std::make_shared<TextureBuffer>(sizeof(glm::mat4) * BatchRenderer::m_MaxObjects);
	}

	void SimpleObjectManager::Add(Renderable* renderable, const glm::mat4& transform)
	{
		uint32_t FoundSlot = -1;
		int FoundMaterialSlot = 0;

		Material material = *renderable->GetMaterial();
		std::shared_ptr<Material> materialPtr = std::make_shared<Material>(material); // FIX IT!!!

		std::shared_ptr<Texture> texturePtr;
		if (renderable->GetMaterial()->Diffuse)
		{
			texturePtr = renderable->GetMaterial()->Diffuse;
		}

		// Check limits (Vertices count, Indices count, Max Materials, Max Transforms, etc...)

		// Finding slot to add object(renderable)
		for (uint32_t slotId = 0; slotId < m_Objects.size(); slotId++)
		{
			// Set FoundMaterialSlot to the next slot from the end
			FoundMaterialSlot = m_Materials[slotId].size();

			// Finding exist material
			for (uint32_t i = 0; i < m_Materials[slotId].size(); i++)
			{
				const Material& material = *m_Materials[slotId][i];  // const or not???
				if ((&material) == renderable->GetMaterial())
				{
					FoundMaterialSlot = i;
					break;
				}
			}

			// If material is found go to add object to buffer
			if (FoundMaterialSlot != m_Materials[slotId].size())
			{
				FoundSlot = slotId;
				break;
			}

			// If material is not found and has no textures in it go to add object to buffer
			if (!renderable->GetMaterial()->HasTextures())
			{
				m_Materials[slotId].push_back(materialPtr);    // FIX IT!!!
				m_MaterialsInfo[slotId].push_back(MaterialInfo{ -1 });
				FoundSlot = slotId;
				break;
			}

			// Finding textures
			// Finding diffuse texture
			if (renderable->GetMaterial()->Diffuse)
			{
				int FoundedTextureSlot = m_Textures[slotId].size();
				for (int i = 0; i < m_Textures[slotId].size(); i++)
				{
					//if (m_Textures[slotId][i] == renderable->GetMaterial()->Diffuse->GetId()) // or
					//const Texture2D& texture = *m_Textures[slotId][i];  // const or not???
					if (m_Textures[slotId][i] == renderable->GetMaterial()->Diffuse)
					{ 
						FoundedTextureSlot = i;
						m_Materials[slotId].push_back(materialPtr);    // FIX IT!!!
						m_MaterialsInfo[slotId].push_back(MaterialInfo{ i });
						//m_MaterialsInfo[FoundMaterialSlot] = MaterialInfo{ i };
						FoundSlot = slotId;
						break;
					}
				}
				if (FoundedTextureSlot != m_Textures[slotId].size())
				{ 
					FoundSlot = slotId;
					break;
				}
				if (FoundedTextureSlot >= m_MaxTexturesToDraw) // FIX TO GL TEXTUREX MAX COUNT minus 1
					continue;

				m_Materials[slotId].push_back(materialPtr);    // FIX IT!!!
				m_MaterialsInfo[slotId].push_back(MaterialInfo{ FoundedTextureSlot });
				m_Textures[slotId].push_back(texturePtr);    // FIX IT!!!
				//m_TexturePointers.push_back(renderable->GetMaterial()->Diffuse);
				//m_MaterialsInfo[FoundMaterialSlot] = MaterialInfo{ FoundedTextureSlot };
			}
			else
			{
				m_Materials[slotId].push_back(materialPtr);    // FIX IT!!!
				m_MaterialsInfo[slotId].push_back(MaterialInfo{ -1 });
				FoundSlot = slotId;
				break;
			}
		}

		// ---------------------------------------- STOPED HERE -----------------------------------------
		if (FoundSlot = -1)
		{
			// Create buffers
			m_Objects.emplace_back();
			m_Vertices.emplace_back();
			m_Materials.emplace_back();
			m_MaterialsInfo.emplace_back();
			m_Textures.emplace_back();
			m_Transformations.emplace_back();
			m_Indices.emplace_back();
			FoundSlot = m_Objects.size() - 1;
			m_Materials[FoundSlot].push_back(materialPtr);    // FIX IT!!!
			if (renderable->GetMaterial()->Diffuse) 
				m_Textures[FoundSlot].push_back(texturePtr);    // FIX IT!!!
			m_MaterialsInfo[FoundSlot].push_back(MaterialInfo{ renderable->GetMaterial()->Diffuse ? 0 : -1 });
		}
		// add data to buffer
		int TransformID = m_Objects[FoundSlot].size();
		m_Objects[FoundSlot].emplace_back(renderable);
		std::vector<Vertex>& vertices = m_Vertices[FoundSlot];
		for (uint32_t i = 0; i < renderable->GetPositions().size(); i++)
		{
			vertices.push_back(Vertex{ renderable->GetPositions()[i], renderable->GetUVs()[i], renderable->GetNormals()[i], renderable->GetTangents()[i], FoundMaterialSlot, TransformID });
		}

		m_Transformations[FoundSlot].push_back(transform);
		
		std::vector<uint32_t>& indices = m_Indices[FoundSlot];
		for (uint32_t i = 0; i < renderable->GetIndices().size(); i++)
		{
			indices.push_back(renderable->GetIndices()[i]);
		}
	}

	bool SimpleObjectManager::Remove(Renderable* renderable)
	{
		LOGE("TODO!!!        void SimpleObjectManager::Remove(Renderable* renderable)");
		return false;
	}

	void SimpleObjectManager::Flush()
	{
		m_Shader->Enable();
		SetUpShader();
		for (uint32_t i = 0; i < m_Objects.size(); i++)
		{
			if (m_Indices[i].size() == 0)
				continue;

			m_VBO->SetData(m_Vertices[i].data(), m_VBO->GetLayout().GetStride() * m_Vertices[i].size());
			m_IBO->SetIndices(m_Indices[i].data(), m_Indices[i].size());
			m_TransformTexture->SetData(m_Transformations[i].data(), sizeof(glm::mat4) * m_Transformations[i].size());

			m_TransformTexture->Bind(0);
			m_VAO->Bind();
			m_IBO->Bind();
			for (int j = 0; j < m_Materials.size(); j++)
				m_Shader->SetMaterial(j, &(*m_Materials[i][j]), &m_MaterialsInfo[i][j]);
			for (int j = 0; j < m_Textures.size(); j++)
				m_Textures[i][j]->Bind(j + m_NotSimpleTextures);

			glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, NULL);
		}
	}

	void SimpleObjectManager::SetUpShader()
	{
		m_Shader->SetUniformMat4("u_ProjectionMatrix", m_Renderer->GetProjection()->GetTransform());
		m_Shader->SetUniformMat4("u_ViewMatrix", m_Renderer->GetCamera()->GetMaxtrix());
		m_Shader->SetUniform3f("u_ViewPos", m_Renderer->GetCamera()->getPosition());
	

		m_Shader->SetUniform3f("u_DirLight.direction", m_Renderer->GetLights()->GetDirection());
		m_Shader->SetUniform3f("u_DirLight.ambient", m_Renderer->GetLights()->GetAmbient());
		m_Shader->SetUniform3f("u_DirLight.diffuse", m_Renderer->GetLights()->GetDiffuse());
		m_Shader->SetUniform3f("u_DirLight.specular", m_Renderer->GetLights()->GetSpecular());
	}

	//////////////////////////////////////////////////////////////////////////////
	////////// BatchRenderer ////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	BatchRenderer::BatchRenderer()
	{

	}

	BatchRenderer::~BatchRenderer()
	{

	}

	void BatchRenderer::OnAttach()
	{
		// Setup ObjectsManager
		m_SimpleObjectManager = new SimpleObjectManager(this);
	}

	void BatchRenderer::OnDetach()
	{
		delete m_SimpleObjectManager;
	}

	void BatchRenderer::OnUpdate()
	{
		Flush();
	}

	void BatchRenderer::BeginScene(Camera* camera, const Projection& projection, Lights* lights)
	{
		m_Camera = camera;
		m_Projection = projection;
		m_Lights = lights;
	}

	void BatchRenderer::Submit(Renderable* renderable)
	{
		m_SimpleObjectManager->Add(renderable, glm::mat4(1.0f));
	}

	void BatchRenderer::Submit(AnimatedRenderable* renderable)
	{
		LOGE("TODO!!!        void BatchRenderer::Submit(AnimatedRenderable* renderable)");
	}

	void BatchRenderer::Submit(std::shared_ptr<Renderable> renderable)
	{
		renderable->Submit(this);
	}

	void BatchRenderer::Flush()
	{
		m_SimpleObjectManager->Flush();
	}

}