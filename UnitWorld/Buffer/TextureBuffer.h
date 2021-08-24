#pragma once

#include <cstdint>

namespace LM
{

	class TextureBuffer
	{
	public:
		TextureBuffer(void* data, uint32_t size);
		TextureBuffer(uint32_t size);
	
		virtual ~TextureBuffer();
	
		void Bind(uint32_t slot = 0) const;
		void Unbind() const;
	
		void SetData(void* data, uint32_t size);
	protected:
		uint32_t m_BufferID;
		uint32_t m_TextureID;
	};

}