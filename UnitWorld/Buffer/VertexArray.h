#pragma once

#include <vector>
#include <memory>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace LM
{

	class VertexArray
	{
	public:
		VertexArray();
		virtual ~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() { return m_VertexBuffers; }
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() { return m_IndexBuffer; }

	protected:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;

		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}