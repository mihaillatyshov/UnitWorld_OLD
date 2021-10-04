#pragma once

#include "Renderer.h"
#include "Core/Shader.h"
#include "Core/Material.h"
#include "Buffer/VertexArray.h"
#include "Buffer/TextureBuffer.h"
#include "Renderables/Transformation.h"

#include <condition_variable>
#include <iostream>
#include <random>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <future>

namespace LM
{

	class BatchRenderer;

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec2 Uv;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		int MaterialId;
		int TransformId;
	};

	class ObjectManager
	{
	public:
		virtual void Add(Renderable* renderable, const glm::mat4& transform) = 0;
		virtual bool Remove(Renderable* renderable) = 0;
	};

	//////////////////////////////////////////////////////////////////////////////
	////////// SimpleObjectManager ///////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	class SimpleObjectManager : public ObjectManager
	{
	public:
		SimpleObjectManager(BatchRenderer* renderer);
		virtual ~SimpleObjectManager() = default;

		void Add(Renderable* renderable, const glm::mat4& transform) override;
		bool Remove(Renderable* renderable) override;

		void Flush();

		void SetUpShader();
	protected:
		int m_MaxTexturesToDraw;
		int m_NotSimpleTextures;

		std::vector<std::vector<Renderable*>> m_Objects;
		std::vector<std::vector<Vertex>> m_Vertices;
		std::vector<std::vector<std::shared_ptr<Material>>> m_Materials;
		std::vector<std::vector<MaterialInfo>> m_MaterialsInfo;
		std::vector<std::vector<std::shared_ptr<Texture>>> m_Textures;
		std::vector<std::vector<glm::mat4>> m_Transformations;
		std::vector<std::vector<uint32_t>> m_Indices;

		std::unique_ptr<Shader> m_Shader;

		std::shared_ptr<VertexArray> m_VAO;
		std::shared_ptr<VertexBuffer> m_VBO;
		std::shared_ptr<IndexBuffer> m_IBO;
		std::shared_ptr<TextureBuffer> m_TransformTexture;
		//std::vector<std::vector<Vertex>>

		BatchRenderer* m_Renderer;
	};

	//////////////////////////////////////////////////////////////////////////////
	////////// BatchRenderer /////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	class BatchRenderer : public Renderer
	{
	public:
		BatchRenderer();
		virtual ~BatchRenderer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;

		virtual void PushTransform(glm::mat4* transform, bool override = false) override {};
		virtual void PopTransform() override {};

		void BeginScene(Camera* camera, const Projection& projection, Lights* lights) override;

		inline Camera* GetCamera() { return m_Camera; }
		inline Projection* GetProjection() { return &m_Projection; }
		inline Lights* GetLights() { return m_Lights; }

		void Submit(std::shared_ptr<Renderable> renderable) override;
	protected:
		void Submit(Renderable* renderable) override;
		void Submit(AnimatedRenderable* renderable) override; // TODO
		void Flush() override;
	public:
		const static uint32_t m_MaxTriangles = 100000;
		const static uint32_t m_MaxObjects = 10000;
		const static uint32_t m_MaxVertices = m_MaxTriangles * 3;
		const static uint32_t m_MaxIndices = m_MaxTriangles * 3;
	protected:
		Camera* m_Camera;
		Projection m_Projection;
		Lights* m_Lights;

		SimpleObjectManager* m_SimpleObjectManager;
	};

}