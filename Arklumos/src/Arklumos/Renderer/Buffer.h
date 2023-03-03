#pragma once

namespace Arklumos
{

	enum class ShaderDataType
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	/*
		Defines a function ShaderDataTypeSize that takes a ShaderDataType enum as input and returns the size in bytes of that data type.

		The function uses a switch statement to determine the size of each possible input type.
		For example, the size of ShaderDataType::Float is 4 bytes, ShaderDataType::Float2 is 8 bytes (4 bytes per float * 2 floats), and so on.
		If an unknown data type is passed in, an assertion is triggered and the function returns 0.
	*/
	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
			return 4;
		case ShaderDataType::Float2:
			return 4 * 2;
		case ShaderDataType::Float3:
			return 4 * 3;
		case ShaderDataType::Float4:
			return 4 * 4;
		case ShaderDataType::Mat3:
			return 4 * 3 * 3;
		case ShaderDataType::Mat4:
			return 4 * 4 * 4;
		case ShaderDataType::Int:
			return 4;
		case ShaderDataType::Int2:
			return 4 * 2;
		case ShaderDataType::Int3:
			return 4 * 3;
		case ShaderDataType::Int4:
			return 4 * 4;
		case ShaderDataType::Bool:
			return 1;
		}

		AK_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	/*
		Defines a BufferElement struct, which is used to represent a single element of a buffer.
		A buffer can be thought of as a contiguous block of memory used to store data, and each element in the buffer represents a single piece of data.

		The BufferElement struct has several member variables:

				Name: a string representing the name of the element
				Type: an enumeration representing the data type of the element (e.g. float, int, bool)
				Size: the size of the element in bytes
				Offset: the offset of the element within the buffer
				Normalized: a boolean indicating whether the data should be normalized

		The struct also has several methods:

				BufferElement(): a default constructor
				BufferElement(ShaderDataType type, const std::string &name, bool normalized = false): a constructor that takes a data type, name, and an optional boolean indicating whether the data should be normalized
				GetComponentCount() const: a method that returns the number of components in the element based on its data type. For example, a float has 1 component, a float2 has 2 components, etc.
	*/
	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		size_t Offset;
		bool Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string &name, bool normalized = false)
				: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:
				return 1;
			case ShaderDataType::Float2:
				return 2;
			case ShaderDataType::Float3:
				return 3;
			case ShaderDataType::Float4:
				return 4;
			case ShaderDataType::Mat3:
				return 3 * 3;
			case ShaderDataType::Mat4:
				return 4 * 4;
			case ShaderDataType::Int:
				return 1;
			case ShaderDataType::Int2:
				return 2;
			case ShaderDataType::Int3:
				return 3;
			case ShaderDataType::Int4:
				return 4;
			case ShaderDataType::Bool:
				return 1;
			}

			AK_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement> &elements)
				: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement> &GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		/*
			Defines a function named CalculateOffsetsAndStride() that calculates the offset and stride of each buffer element in a vertex buffer layout.

			The function iterates over the m_Elements vector which contains information about each buffer element.
			It starts by initializing the offset variable to zero and the m_Stride variable to zero.
			Then, for each element in the vector, the function sets the Offset field of the element to the current offset value, which starts at zero and is incremented by the Size of the current element.
			The Size field of each element is previously calculated using the ShaderDataTypeSize function.

			After processing all elements, the m_Stride variable is set to the total size of the vertex buffer layout, which is the sum of all element sizes.

			In summary, this function calculates the byte offset of each buffer element and the total size of the buffer layout in bytes.
			These values are used later in the application to upload vertex data to the graphics API.
		*/
		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			m_Stride = 0;
			for (auto &element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout &GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout &layout) = 0;

		static VertexBuffer *Create(float *vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer *Create(uint32_t *indices, uint32_t size);
	};

}