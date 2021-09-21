#include "TextureBuffer.h"

#include <GL/glew.h>
#include <glm.hpp>
#include "GLCall.h"

namespace LM
{

	TextureBuffer::TextureBuffer(void* data, uint32_t size)
	{
		GLCall(glGenBuffers(1, &m_BufferID));
		GLCall(glGenTextures(1, &m_TextureID));

		GLCall(glBindBuffer(GL_TEXTURE_BUFFER, m_BufferID));
		GLCall(glBufferData(GL_TEXTURE_BUFFER, size, data, GL_DYNAMIC_DRAW));

		GLCall(glBindTexture(GL_TEXTURE_BUFFER, m_TextureID));
		GLCall(glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_BufferID));
	}
	
	TextureBuffer::TextureBuffer(uint32_t size)
	{
		glGenBuffers(1, &m_BufferID);
		glBindBuffer(GL_TEXTURE_BUFFER, m_BufferID);
		glBufferData(GL_TEXTURE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_BUFFER, m_TextureID);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_BufferID);
	}
	
	TextureBuffer::~TextureBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
		glDeleteTextures(1, &m_TextureID);
	}

	void TextureBuffer::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_BUFFER, m_TextureID);
	}

	void TextureBuffer::Unbind() const
	{
		glBindTexture(GL_TEXTURE_BUFFER, 0);
	}

	void TextureBuffer::SetData(void* data, uint32_t size)
	{
		glBindBuffer(GL_TEXTURE_BUFFER, m_BufferID);
		glBufferSubData(GL_TEXTURE_BUFFER, 0, size, data);
	}

}