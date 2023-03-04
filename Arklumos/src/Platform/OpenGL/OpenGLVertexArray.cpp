#include "akpch.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Arklumos
{

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
			return GL_FLOAT;
		case ShaderDataType::Float2:
			return GL_FLOAT;
		case ShaderDataType::Float3:
			return GL_FLOAT;
		case ShaderDataType::Float4:
			return GL_FLOAT;
		case ShaderDataType::Mat3:
			return GL_FLOAT;
		case ShaderDataType::Mat4:
			return GL_FLOAT;
		case ShaderDataType::Int:
			return GL_INT;
		case ShaderDataType::Int2:
			return GL_INT;
		case ShaderDataType::Int3:
			return GL_INT;
		case ShaderDataType::Int4:
			return GL_INT;
		case ShaderDataType::Bool:
			return GL_BOOL;
		}

		AK_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		AK_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		AK_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		AK_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		AK_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer)
	{
		AK_PROFILE_FUNCTION();

		AK_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		// This line retrieves the layout of the vertex buffer, which specifies the format of the vertex data.
		const auto &layout = vertexBuffer->GetLayout();
		for (const auto &element : layout)
		{
			// This line enables the vertex attribute at the current index.
			glEnableVertexAttribArray(m_VertexBufferIndex);

			/*
				Specifies how OpenGL should interpret the data at the current vertex attribute. It takes six arguments:

					m_VertexBufferIndex: The index of the current vertex attribute.
					element.GetComponentCount(): The number of components in the current element.
					ShaderDataTypeToOpenGLBaseType(element.Type): The data type of the current element, translated from the custom shader data type to the corresponding OpenGL base type.
					element.Normalized ? GL_TRUE : GL_FALSE: Whether the data should be normalized.
					layout.GetStride(): The distance in bytes between consecutive vertices.
					(const void *)(uintptr_t)element.Offset: A pointer to the offset of the current element within the vertex data. The uintptr_t cast is used to convert the offset to a pointer-sized integer, which can be used as a void pointer.
			*/
			glVertexAttribPointer(m_VertexBufferIndex,
														element.GetComponentCount(),
														ShaderDataTypeToOpenGLBaseType(element.Type),
														element.Normalized ? GL_TRUE : GL_FALSE,
														layout.GetStride(),
														(const void *)(uintptr_t)element.Offset);
			m_VertexBufferIndex++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer)
	{
		AK_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}