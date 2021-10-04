#pragma once

#include <cstdint>

namespace LM
{

	class IndexBuffer
	{
	public:
		IndexBuffer(uint32_t count);
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;
		
		void SetIndices(uint32_t* indices, uint32_t count);
		
		uint32_t GetCount() const { return m_Count; }

	protected:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

}