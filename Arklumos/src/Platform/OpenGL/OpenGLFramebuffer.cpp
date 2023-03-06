#include "akpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Arklumos
{

	static const uint32_t s_MaxFramebufferSize = 8192;

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification &spec)
			: m_Specification(spec)
	{
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		// Deletes a single framebuffer object, identified by the m_RendererID member variable of the OpenGLFramebuffer object. A framebuffer object is an OpenGL object that contains a set of buffers for storing color, depth, and stencil information used for rendering.
		glDeleteFramebuffers(1, &m_RendererID);
		// Delete the two texture objects used for color and depth attachments, identified by the m_ColorAttachment and m_DepthAttachment member variables, respectively. Texture objects are used to store and manipulate texture images in OpenGL, and they can be used as attachments to a framebuffer object for rendering
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		/*
			Checking if the m_RendererID member variable of the current object is not equal to zero. If m_RendererID is not zero, it means that the object has already been initialized with a valid OpenGL identifier for a framebuffer object.

			The purpose of this check is to ensure that the destructor doesn't try to delete any OpenGL resources that haven't been created yet.

			If m_RendererID is not zero, the code proceeds to delete the framebuffer object and its color and depth attachments using the glDeleteFramebuffers and glDeleteTextures functions from the OpenGL library, respectively.
			These functions will deallocate the resources previously allocated by the object when it was initialized.

			In summary, this code checks if the object has a valid OpenGL identifier before deleting the framebuffer object and its attachments to free the resources previously allocated for the object.
			If the object doesn't have a valid identifier, the code simply does nothing. This helps prevent errors and crashes that can occur when trying to delete resources that haven't been allocated yet.
		*/
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_DepthAttachment);
		}

		/*
			Initializing an OpenGL framebuffer object by creating and binding it to a render target. It is also creating and attaching a color texture to the framebuffer object.

			The first line of code creates a single framebuffer object using the glCreateFramebuffers function from the OpenGL library. The function takes two parameters: the number of framebuffer objects to create (in this case, just one), and a pointer to an array of framebuffer object names to be created. The function returns the name of the newly created framebuffer object in the variable m_RendererID.

			The second line of code binds the newly created framebuffer object to the render target using the glBindFramebuffer function. The function takes two parameters: the target (in this case, GL_FRAMEBUFFER), and the name of the framebuffer object to bind (m_RendererID in this case).

			The next three lines of code create a new 2D texture object using the glCreateTextures function. The function takes three parameters: the type of texture object to create (in this case, GL_TEXTURE_2D), the number of texture objects to create (in this case, just one), and a pointer to an array of texture object names to be created. The function returns the name of the newly created texture object in the variable m_ColorAttachment.

			The fifth line of code binds the newly created texture object to the current texture unit using the glBindTexture function. The function takes two parameters: the target (in this case, GL_TEXTURE_2D), and the name of the texture object to bind (m_ColorAttachment in this case).

			The sixth line of code sets the data for the texture object using the glTexImage2D function. The function takes nine parameters: the type of texture object being modified (in this case, GL_TEXTURE_2D), the level of detail (in this case, 0), the internal format of the texture (in this case, GL_RGBA8), the width and height of the texture (specified by m_Specification.Width and m_Specification.Height), the border (0 in this case), the format of the pixel data (in this case, GL_RGBA), the data type of the pixel data (in this case, GL_UNSIGNED_BYTE), and a pointer to the pixel data (in this case, nullptr since we're not setting any data yet).

			The seventh and eighth lines of code set the filtering parameters for the texture object using the glTexParameteri function. The function takes three parameters: the type of texture object being modified (in this case, GL_TEXTURE_2D), the parameter to set (in this case, GL_TEXTURE_MIN_FILTER and GL_TEXTURE_MAG_FILTER), and the value to set the parameter to (in this case, GL_LINEAR).

			In summary, this code is initializing an OpenGL framebuffer object by creating and binding it to a render target, and creating and attaching a color texture to the framebuffer object.
		*/
		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		AK_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			AK_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
			return;
		}

		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

}