#include "VertexArray.h"

#include <GL/glew.h>
#include "Core/Core.h"

namespace LM
{

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
			case ShaderDataType::Bool:     return GL_BOOL;
		}

		CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	static inline bool IsNotFloat(const BufferElement& element)
	{
		return ShaderDataTypeToOpenGLBaseType(element.Type) == GL_BYTE ||
			   ShaderDataTypeToOpenGLBaseType(element.Type) == GL_UNSIGNED_BYTE ||
			   ShaderDataTypeToOpenGLBaseType(element.Type) == GL_SHORT ||
			   ShaderDataTypeToOpenGLBaseType(element.Type) == GL_UNSIGNED_SHORT ||
			   ShaderDataTypeToOpenGLBaseType(element.Type) == GL_INT ||
			   ShaderDataTypeToOpenGLBaseType(element.Type) == GL_UNSIGNED_INT;
	}

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no Layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const BufferLayout& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(m_VertexBufferIndex);
			if (IsNotFloat(element))
			{
				glVertexAttribIPointer(m_VertexBufferIndex, 
					element.GetComponentCount(), 
					ShaderDataTypeToOpenGLBaseType(element.Type), 
					layout.GetStride(), 
					(const void*)element.Offset);
			}
			else
			{
				glVertexAttribPointer(m_VertexBufferIndex, 
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE, 
					layout.GetStride(), 
					(const void*)element.Offset);
			}
			m_VertexBufferIndex++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}


}