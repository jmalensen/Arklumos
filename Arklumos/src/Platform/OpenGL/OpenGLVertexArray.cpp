#include "akpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Arklumos
{

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Arklumos::ShaderDataType::Float:
			return GL_FLOAT;
		case Arklumos::ShaderDataType::Float2:
			return GL_FLOAT;
		case Arklumos::ShaderDataType::Float3:
			return GL_FLOAT;
		case Arklumos::ShaderDataType::Float4:
			return GL_FLOAT;
		case Arklumos::ShaderDataType::Mat3:
			return GL_FLOAT;
		case Arklumos::ShaderDataType::Mat4:
			return GL_FLOAT;
		case Arklumos::ShaderDataType::Int:
			return GL_INT;
		case Arklumos::ShaderDataType::Int2:
			return GL_INT;
		case Arklumos::ShaderDataType::Int3:
			return GL_INT;
		case Arklumos::ShaderDataType::Int4:
			return GL_INT;
		case Arklumos::ShaderDataType::Bool:
			return GL_BOOL;
		}

		AK_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer)
	{
		AK_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		// This variable keeps track of the index of the currently enabled vertex attribute.
		uint32_t index = 0;
		// This line retrieves the layout of the vertex buffer, which specifies the format of the vertex data.
		const auto &layout = vertexBuffer->GetLayout();
		for (const auto &element : layout)
		{
			// This line enables the vertex attribute at the current index.
			glEnableVertexAttribArray(index);

			/*
				Specifies how OpenGL should interpret the data at the current vertex attribute. It takes six arguments:

					index: The index of the current vertex attribute.
					element.GetComponentCount(): The number of components in the current element.
					ShaderDataTypeToOpenGLBaseType(element.Type): The data type of the current element, translated from the custom shader data type to the corresponding OpenGL base type.
					element.Normalized ? GL_TRUE : GL_FALSE: Whether the data should be normalized.
					layout.GetStride(): The distance in bytes between consecutive vertices.
					(const void *)(uintptr_t)element.Offset: A pointer to the offset of the current element within the vertex data. The uintptr_t cast is used to convert the offset to a pointer-sized integer, which can be used as a void pointer.
			*/
			glVertexAttribPointer(index,
														element.GetComponentCount(),
														ShaderDataTypeToOpenGLBaseType(element.Type),
														element.Normalized ? GL_TRUE : GL_FALSE,
														layout.GetStride(),
														(const void *)(uintptr_t)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}