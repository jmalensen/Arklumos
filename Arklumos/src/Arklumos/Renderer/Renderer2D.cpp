#include "akpch.h"
#include "Arklumos/Renderer/Renderer2D.h"

#include "Arklumos/Renderer/VertexArray.h"
#include "Arklumos/Renderer/Shader.h"
#include "Arklumos/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Arklumos
{

	/*
		Defines a struct called QuadVertex which represents the vertex format of a textured quad.

		The QuadVertex struct has the following members:

				Position - A 3-component vector that represents the position of the vertex in 3D space.
				Color - A 4-component vector that represents the color of the vertex.
				TexCoord - A 2-component vector that represents the texture coordinates of the vertex.
				TexIndex - A float value that represents the index of the texture to be used for this vertex. This is useful when using a texture atlas, where multiple textures are stored in a single texture image.
				TilingFactor - A float value that represents the factor by which the texture should be tiled. This is useful for applying textures to larger surfaces without having to stretch the texture.

		Together, these members define the complete vertex format for a textured quad
	*/
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	/*
		Defines a struct called Renderer2DData that holds data related to the 2D renderer. Here's a breakdown of what each member variable does:

			MaxQuads: A constant that defines the maximum number of quads that can be rendered at once.
			MaxVertices: A constant that defines the maximum number of vertices that can be used to render quads.
			MaxIndices: A constant that defines the maximum number of indices that can be used to render quads.
			MaxTextureSlots: A constant that defines the maximum number of texture slots available for rendering.
			QuadVertexArray: A smart pointer to a vertex array object that holds the vertex and index buffers for rendering quads.
			QuadVertexBuffer: A smart pointer to a vertex buffer object that holds the quad vertex data.
			TextureShader: A smart pointer to a shader object that is used to render textured quads.
			WhiteTexture: A smart pointer to a texture object that is used as a fallback texture when no other texture is available.
			QuadIndexCount: The number of quad indices currently used.
			QuadVertexBufferBase: A pointer to the beginning of the quad vertex buffer.
			QuadVertexBufferPtr: A pointer to the current position in the quad vertex buffer.
			TextureSlots: An array of smart pointers to texture objects used for rendering textured quads.
			TextureSlotIndex: The index of the next available texture slot.
			QuadVertexPositions: An array that holds the positions of the vertices of a quad.
			Stats: A struct that holds statistics about the renderer's performance.
	*/
	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex *QuadVertexBufferBase = nullptr;
		QuadVertex *QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		// AK_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();

		/*
			Creates a Vertex Buffer and set its layout to be used for storing data of quad vertices in memory.

			The first line is creating the Vertex Buffer object and storing it in the static Renderer2DData struct's QuadVertexBuffer member. The size of the buffer is calculated by multiplying the maximum number of vertices that can be drawn (s_Data.MaxVertices) with the size of a single quad vertex (sizeof(QuadVertex)).

			The second line is setting the layout of the Vertex Buffer. It specifies the data type and name of each attribute for a quad vertex. The attributes are defined using the ShaderDataType enum and a string identifier. In this case, a_Position, a_Color, a_TexCoord, a_TexIndex, and a_TilingFactor are the attributes for the vertex data.

			Finally adds the vertex buffer to the quad vertex array that is stored in the Renderer2DData struct's QuadVertexArray member
		*/
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({{ShaderDataType::Float3, "a_Position"},
																				{ShaderDataType::Float4, "a_Color"},
																				{ShaderDataType::Float2, "a_TexCoord"},
																				{ShaderDataType::Float, "a_TexIndex"},
																				{ShaderDataType::Float, "a_TilingFactor"}});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		// New QuadVertex
		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t *quadIndices = new uint32_t[s_Data.MaxIndices];

		/*
			Initializes the indices of a quad vertex array for rendering. Each quad consists of four vertices, and each vertex has a unique index.
			The indices are set up in the order that OpenGL draws triangles, which is in a counter-clockwise winding order.

			The offset variable keeps track of the current index of the first vertex in each quad.
			The loop increments i by 6 because there are two triangles per quad, and each triangle has three indices.
			The indices are set up in the following order:
				2----3
				|    |
				|    |
				0----1
			The first triangle is made up of indices 0, 1, and 2, and the second triangle is made up of indices 2, 3, and 0.
			This winding order ensures that the front face of the quad is facing the camera, which is important for correctly applying lighting and other effects.
		*/
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		/*
			Creates an index buffer and passes in the quadIndices array, which contains the indices of each quad.
			The size of the array is s_Data.MaxIndices, which is the maximum number of indices that can be stored in the index buffer.

			Next, it sets the created index buffer object to the vertex array object by calling the SetIndexBuffer method on the s_Data.QuadVertexArray object and passing in the created index buffer object.

			Finally, it deallocates the memory allocated for the quadIndices array using the delete[] operator
		*/
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		/*
			Creates a 1x1 white texture and sets its data to be a single pixel with a value of 0xffffffff, which represents white in RGBA format.

			The Texture2D::Create() function creates a texture object with the specified width and height, and returns a reference to the texture.

			The s_Data.WhiteTexture->SetData() function sets the texture's data to the specified value.
			The first argument is a pointer to the data, and the second argument is the size of the data in bytes.
			In this case, the data is a single 32-bit integer representing the color of a single pixel.
		*/
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		/*
			Initializes an integer array called samplers with values from 0 to s_Data.MaxTextureSlots - 1.
			The purpose of this array is to be used as an argument when binding multiple textures to the shader.
			Each element of the array represents a texture unit, which is a location in the GPU where a texture can be bound.
			The value of each element represents the index of the texture unit.
			By initializing the array with consecutive integers starting from 0, each element represents a unique texture unit
		*/
		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}

		/*
			Creates a new shader program by loading the shader source code from a file named "Texture.glsl" located in the "assets/shaders/" directory. The newly created shader program is then bound for use.

			Next, an integer array samplers of size s_Data.MaxTextureSlots is created, where each element is initialized to its own index value.
			Then, the integer array samplers is passed to the shader program's uniform u_Textures as an integer array.

			This allows the shader program to access multiple textures using texture units. Each element in the samplers array represents a texture unit, which can be used to bind textures to that unit.
			When a texture is bound to a specific texture unit, the integer value representing that texture unit is passed as the TexIndex value of the QuadVertex struct for each vertex.
			This way, the shader program can look up the correct texture for each vertex based on its TexIndex value
		*/
		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		// Set first texture slot to 0
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		/*
			Initializes the QuadVertexPositions array in the Renderer2DData struct with the positions of the vertices of a quad.

			The quad is defined with four vertices, which are stored in s_Data.QuadVertexPositions. Each vertex is represented by a glm::vec4 vector with the following values:

					Vertex 0: (-0.5f, -0.5f, 0.0f, 1.0f)
					Vertex 1: (0.5f, -0.5f, 0.0f, 1.0f)
					Vertex 2: (0.5f, 0.5f, 0.0f, 1.0f)
					Vertex 3: (-0.5f, 0.5f, 0.0f, 1.0f)

			These values represent the position of each vertex in 3D space (x, y, and z coordinates) and a homogeneous coordinate (w).
			In this case, the z-coordinate is 0 because the quad lies on the XY-plane.
			The homogeneous coordinate is usually set to 1 for regular 3D positions, but here it is set to 1 to make use of a transformation matrix in the vertex shader
		*/
		s_Data.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
		s_Data.QuadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
		s_Data.QuadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
		s_Data.QuadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
	}

	void Renderer2D::Shutdown()
	{
		// AK_PROFILE_FUNCTION();

		delete[] s_Data.QuadVertexBufferBase;
	}

	/*
		Begins a new rendering scene with the given OrthographicCamera by binding the texture shader and setting the view projection matrix uniform to the view projection matrix of the given camera.

		s_Data.QuadIndexCount is set to zero to start counting the number of indices.
		s_Data.QuadVertexBufferPtr is set to s_Data.QuadVertexBufferBase, which is the pointer to the base of the quad vertex buffer, indicating that the next vertex data should be written at the beginning of the vertex buffer.

		s_Data.TextureSlotIndex is set to 1 to start assigning texture slots from the index 1, because the index 0 is reserved for the white texture.
	*/
	void Renderer2D::BeginScene(const OrthographicCamera &camera)
	{
		// AK_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(const Camera &camera, const glm::mat4 &transform)
	{
		// AK_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	/*
		End a 2D rendering scene. It is responsible for finalizing the rendering state and flushing all the rendered data to the screen.

		The function first calculates the total size of the rendered data by subtracting the starting address of the vertex buffer from the current address of the buffer pointer.
		The resulting size is then used to set the data of the vertex buffer using the SetData() method of the buffer.

		After setting the data of the vertex buffer, the function then calls Flush(), which is responsible for rendering all the data in the buffer to the screen
	*/
	void Renderer2D::EndScene()
	{
		// AK_PROFILE_FUNCTION();

		uint32_t dataSize = (uint32_t)((uint8_t *)s_Data.QuadVertexBufferPtr - (uint8_t *)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	/*
		Draw all the quads that have been submitted to the renderer since the last call to Flush.

		The function first checks whether there are any quads to draw by checking if the s_Data.QuadIndexCount is equal to 0.
		If there are no quads to draw, the function simply returns.

		If there are quads to draw, the function proceeds to bind the textures that were used in the submitted quads.
		This is done by iterating over the s_Data.TextureSlots array, which contains pointers to the texture objects used in the submitted quads, and calling the Bind method on each texture object, passing in the texture slot index as a parameter.
		This ensures that each texture is bound to the correct texture slot for rendering.

		Finally, the function calls the DrawIndexed method of the RenderCommand class, passing in the vertex array object and the number of indices to draw.
		This is done to actually render the quads on the screen. After the draw call is completed, the draw call count in the s_Data.Stats struct is incremented to keep track of the number of draw calls made by the renderer
	*/
	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount == 0)
		{
			return; // Nothing to draw
		}

		// Bind textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	/*
		End the current scene and reset the internal state of the 2D renderer for the next frame.

		First, it calls the EndScene function to finalize any pending draw calls and uploads the vertex data to the GPU.

		Then, it resets the following state variables:

				QuadIndexCount: sets it to 0 to clear the accumulated quad index count from the previous frame.
				QuadVertexBufferPtr: sets it to the beginning of the vertex buffer to prepare for new vertex data.
				TextureSlotIndex: sets it to 1, indicating that the first texture slot (index 0) is already occupied by the white texture and the next available texture slot index is 1.

		This function is typically called at the end of each frame to prepare the renderer for the next frame's drawing operations
	*/
	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
	{
		DrawQuad({position.x, position.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color)
	{
		// AK_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor)
	{
		DrawQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor)
	{
		// AK_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4 &transform, const glm::vec4 &color)
	{
		// AK_PROFILE_FUNCTION();

		/*
			Prepare to draw a quad with a white texture.

				quadVertexCount is a constant that represents the number of vertices in a quad.
				textureIndex is a float that represents the index of the texture to be used. In this case, it is set to 0, which means the white texture will be used.
				textureCoords is an array of glm::vec2 vectors that define the texture coordinates for each vertex of the quad. Each vertex will have a different set of texture coordinates, with the first vertex having (0, 0), the second vertex having (1, 0), the third vertex having (1, 1), and the fourth vertex having (0, 1).
				tilingFactor is a float that determines how many times the texture is tiled across the quad.
		*/
		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
		const float tilingFactor = 1.0f;

		// Checks if there are enough indices left to draw the quad. If there are not, the FlushAndReset() function is called to draw the existing batch of quads and reset the vertex buffer and index count for the next batch
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			FlushAndReset();
		}

		/*
			Create a quad and add its vertices to the vertex buffer. The quad is transformed by a given position and size using a 4x4 transformation matrix, which is created using the glm::translate and glm::scale functions.

			The for loop loops through the four vertices of the quad and sets the position, color, texture coordinates, texture index, and tiling factor for each vertex.
			The position of each vertex is transformed by the given transform matrix transform * s_Data.QuadVertexPositions[i].

			After the vertices are added to the vertex buffer, the quad index count is incremented by 6 since each quad is made up of two triangles, each consisting of three vertices.
			Finally, the quad count statistic is incremented in the renderer's data.
		*/
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4 &transform, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor)
	{
		// AK_PROFILE_FUNCTION();

		/*
			Initializes a constant array textureCoords with four glm::vec2 values representing the texture coordinates for each vertex of a quad.
			The coordinates are defined in counter-clockwise order starting from the bottom-left vertex.

			The code then checks if the current number of quad indices has exceeded the maximum number of indices allowed in the renderer's data object (Renderer2DData::MaxIndices).
			If it has, the FlushAndReset() function is called, which sends the current batch of quads to be drawn and resets the renderer's state to start a new batch.
			This check is necessary because the number of indices is limited by the underlying graphics API and hardware.
		*/
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			FlushAndReset();
		}

		/*
			Determine the texture slot index of a given texture.

			The textureIndex variable is initialized to 0.0f, which corresponds to the first texture slot index.
			Then, a loop is used to iterate over all the texture slots in the s_Data struct, starting from index 1 (since index 0 is always reserved for the white texture).

			For each texture slot, the code checks whether the texture pointer stored in the texture slot is equal to the pointer of the given texture parameter.
			If the two pointers are equal, it means that the texture has already been added to the texture slots and its index can be determined by subtracting 1 from the texture slot index (since the first slot is reserved for the white texture).
			The index is then stored in the textureIndex variable and the loop is exited using the break statement.

			If the texture is not found in the existing texture slots, the textureIndex remains at its initial value of 0.0f, indicating that a new texture slot needs to be created for this texture.
		*/
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		/*
			Checks if the texture index is equal to zero, which means that the texture has not been previously bound.
			If the texture index is zero, the code checks whether there is space for another texture slot in the Renderer2DData::MaxTextureSlots array.
			If there is no space, the FlushAndReset function is called to clear the current buffer and start anew.

			If there is space, the texture is assigned to the s_Data.TextureSlots array at the s_Data.TextureSlotIndex position and the texture index is set to this value.
			The s_Data.TextureSlotIndex is then incremented to reserve the next available texture slot for future use.
		*/
		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
			{
				FlushAndReset();
			}

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color)
	{
		DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color)
	{
		// AK_PROFILE_FUNCTION();

		/*
			Creates a 4x4 transformation matrix transform that combines a translation, rotation, and scaling. Here's what each part of the matrix does:

			glm::translate(glm::mat4(1.0f), position): This creates a translation matrix that will move an object to the position specified. The glm::mat4(1.0f) creates a 4x4 identity matrix that will be multiplied by the translation matrix.

			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}): This creates a rotation matrix that will rotate an object rotation degrees around the z-axis. The glm::mat4(1.0f) creates another 4x4 identity matrix that will be multiplied by the rotation matrix.

			glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f}): This creates a scaling matrix that will scale an object by the size specified. The glm::mat4(1.0f) creates yet another 4x4 identity matrix that will be multiplied by the scaling matrix.

			All three matrices are multiplied together using the * operator to create a final transformation matrix that combines translation, rotation, and scaling. This matrix is then assigned to the transform variable.
		*/
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor)
	{
		DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor)
	{
		// AK_PROFILE_FUNCTION();

		/*
			Defines a transformation matrix, which can be used to transform the position, rotation, and scale of an object in 2D space.

			glm::mat4 is a 4x4 matrix data type provided by the GLM library. The glm::translate function creates a 4x4 translation matrix that moves an object by a specified position vector.
			The glm::rotate function creates a 4x4 rotation matrix that rotates an object by a specified rotation angle around the z-axis. The glm::scale function creates a 4x4 scaling matrix that scales an object by a specified size vector.

			The resulting transformation matrix is created by multiplying the translation, rotation, and scale matrices together, in that order.
			The resulting matrix will have the effect of first translating the object to the position location, then rotating it around the z-axis by rotation degrees, and finally scaling it by size in the x and y directions (with no scaling in the z direction).

			Once the transformation matrix is defined, it can be applied to the vertex positions of a 2D object to transform it in space.
		*/
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

}