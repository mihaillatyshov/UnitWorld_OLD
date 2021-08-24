#pragma once

#include <cstdint>

#include "BufferLayout.h"

namespace LM
{

	class VertexBuffer
	{
	public:
		VertexBuffer(uint32_t size);
		VertexBuffer(void* data, uint32_t size);
		virtual ~VertexBuffer();
		
		void Bind() const;
		void Unbind() const;

		void SetData(void* data, uint32_t size);

		const BufferLayout& GetLayout() const { return m_Layout; }
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

	protected:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

}