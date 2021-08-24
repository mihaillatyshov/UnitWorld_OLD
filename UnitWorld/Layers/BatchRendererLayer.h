#pragma once

#include "Layer.h"

#include "Buffer/VertexArray.h"
#include "Buffer/TextureBuffer.h"
#include "Core/Shader.h"
#include "Renderers/BatchRenderer.h"
#include "Renderables/Sprite.h"

namespace LM
{

	class BatchRendererLayer : public Layer
	{
	public:
		BatchRendererLayer();
		~BatchRendererLayer();
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;

		void OnImGuiRender() override;
	protected:
		std::shared_ptr<VertexArray> m_VAO;
		std::shared_ptr<VertexBuffer> m_VBO;
		std::shared_ptr<IndexBuffer> m_Indieces;
		std::shared_ptr<TextureBuffer> m_TextureBuffer;
		glm::vec3 m_Position;
		glm::mat4 m_Transform;

		//std::shared_ptr<Texture2D> m_Texture;
		std::shared_ptr<Material> m_Material;
		std::shared_ptr<Sprite> m_Sprite;
		
		Projection m_Projection = Projection(glm::perspective(80.0f, 16.0f / 9.0f, 0.01f, 100.0f));

		Renderer* m_Renderer;
	};

}