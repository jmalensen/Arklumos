#include "akpch.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "Arklumos/Core/Timer.h"

namespace Arklumos
{

	namespace Utils
	{

		static GLenum ShaderTypeFromString(const std::string &type)
		{
			if (type == "vertex")
			{
				return GL_VERTEX_SHADER;
			}
			if (type == "fragment" || type == "pixel")
			{
				return GL_FRAGMENT_SHADER;
			}

			AK_CORE_ASSERT(false, "Unknown shader type!");
			return 0;
		}

		static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:
				return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER:
				return shaderc_glsl_fragment_shader;
			}
			AK_CORE_ASSERT(false);
			return (shaderc_shader_kind)0;
		}

		static const char *GLShaderStageToString(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:
				return "GL_VERTEX_SHADER";
			case GL_FRAGMENT_SHADER:
				return "GL_FRAGMENT_SHADER";
			}
			AK_CORE_ASSERT(false);
			return nullptr;
		}

		static const char *GetCacheDirectory()
		{
			// TODO: make sure the assets directory is valid
			return "assets/cache/shader/opengl";
		}

		static void CreateCacheDirectoryIfNeeded()
		{
			std::string cacheDirectory = GetCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory))
			{
				std::filesystem::create_directories(cacheDirectory);
			}
		}

		static const char *GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:
				return ".cached_opengl.vert";

			case GL_FRAGMENT_SHADER:
				return ".cached_opengl.frag";
			}
			AK_CORE_ASSERT(false);
			return "";
		}

		static const char *GLShaderStageCachedVulkanFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:
				return ".cached_vulkan.vert";

			case GL_FRAGMENT_SHADER:
				return ".cached_vulkan.frag";
			}
			AK_CORE_ASSERT(false);
			return "";
		}
	}

	/*
		Constructor for the OpenGLShader with a filepath

		First, the function reads the contents of the file using the ReadFile function. Then, it calls the PreProcess function which preprocesses the shader source code and separates the shader code into individual shader sources (vertex, fragment, geometry, etc.) that can be compiled separately. Finally, the Compile function is called to compile the shader sources.

		After compiling the shader sources, the constructor extracts the name of the shader from the file path by finding the last occurrence of a slash (/ or \) character to determine the beginning of the filename, and the last occurrence of a dot (.) character to determine the end of the filename extension. The substring of the file path between these two indices is set as the name of the shader using the substr function
	*/
	OpenGLShader::OpenGLShader(const std::string &filepath)
			: m_FilePath(filepath)
	{
		// AK_PROFILE_FUNCTION();

		Utils::CreateCacheDirectoryIfNeeded();

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);

		{
			Timer timer;
			CompileOrGetVulkanBinaries(shaderSources);
			CompileOrGetOpenGLBinaries();
			CreateProgram();
			AK_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
		}

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	/*
		Constructor for the OpenGLShader that takes in a shader name, vertex shader source code, and fragment shader source code. It initializes the m_Name member variable with the given name parameter.

		Then, it creates an std::unordered_map called sources, which maps a GLenum value (representing the shader type) to the corresponding shader source code.
		The vertex shader source code is mapped to GL_VERTEX_SHADER, and the fragment shader source code is mapped to GL_FRAGMENT_SHADER.

		Finally, the Compile function is called with the sources map as a parameter, which compiles and links the shader program
	*/
	OpenGLShader::OpenGLShader(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc)
			: m_Name(name)
	{
		// AK_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		CompileOrGetVulkanBinaries(sources);
		CompileOrGetOpenGLBinaries();
		CreateProgram();
	}

	OpenGLShader::~OpenGLShader()
	{
		// AK_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	/*
		Defines ReadFile which takes a std::string parameter filepath indicating the path to a file to be read. It returns a std::string that contains the contents of the file.

		The function first declares an empty std::string called result to store the contents of the file.
		It then uses the std::ifstream class to open the file in binary mode using the given file path.
		If the file is successfully opened, it first gets the size of the file using in.tellg(), and then resizes result to the size of the file. It then reads the contents of the file into result.
		If the file is not opened successfully, it outputs an error message using AK_CORE_ERROR.

		Finally, the function returns result which is the contents of the file as a string
	*/
	std::string OpenGLShader::ReadFile(const std::string &filepath)
	{
		// AK_PROFILE_FUNCTION();

		std::string result;
		// Warning: ifstream closes itself due to RAII
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				AK_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			AK_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	/*
		Helper function for preprocessing shader source code.
		The function takes in a string source, which represents the entire source code for a shader program, and returns an unordered_map that maps GLenum (OpenGL enum representing shader types) to shader source code.

		The function searches for special lines that start with #type in the source code. These lines specify the type of the shader program that follows.
		The function extracts the shader source code for each type of shader and adds it to the shaderSources map.

		The while loop keeps searching for the next #type line until it reaches the end of the source code.
	*/
	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string &source)
	{
		// AK_PROFILE_FUNCTION();

		// Declare an empty unordered_map shaderSources.
		std::unordered_map<GLenum, std::string> shaderSources;

		// Define a string typeToken that will be used to search for #type lines in the shader source code.
		const char *typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);

		// Find the first #type line in the shader source code using source.find(typeToken, 0). This returns the position of the first occurrence of the #type line in the source string, starting at index 0.
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			// If #type line is found, find the end of the line (i.e., the position of the first occurrence of a newline character \r or \n after the #type line). This is done using source.find_first_of("\r\n", pos).
			size_t eol = source.find_first_of("\r\n", pos);
			AK_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			// Check that the type string is a valid shader type using ShaderTypeFromString(type). This function converts the string type to its corresponding GLenum value. If type is not a valid shader type, an assertion is triggered.
			AK_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified");

			// Start of shader code after shader type declaration line (This is done by finding the position of the first non-newline character after the end of the #type line)
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			AK_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			// Start of next shader type declaration line
			// Find the next #type line in the shader source code, starting at the position of the first non-newline character after the end of the previous shader code block.
			// If there is no next #type line, the shader code block extends to the end of the shader source code.
			pos = source.find(typeToken, nextLinePos);

			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);

			// Repeat until there are no more #type lines in the shader source code.
		}

		return shaderSources;
	}

	// Compiles and attaches shader objects to the program
	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string> &shaderSources)
	{
		/*
			First, a new program is created using glCreateProgram(), which returns a unique identifier for the program. Then, the code checks if there are only two shaders in shaderSources.
			If there are more than two, it asserts to stop the execution.

			A loop is then used to create and compile each shader object from shaderSources. For each shader, a new shader object is created using glCreateShader().
			The shader object is then set up with the shader source code using glShaderSource() and compiled using glCompileShader().

			After compiling the shader object, the code checks whether the compilation was successful by calling glGetShaderiv() with GL_COMPILE_STATUS as the parameter.
			If the shader compilation was not successful, an error message is printed, and the shader object is deleted.

			If the compilation is successful, the shader object is attached to the program using glAttachShader().
			The shader object's unique identifier is also stored in the glShaderIDs array.

			Once all shaders have been compiled and attached to the program, the program is linked using glLinkProgram().
		*/
		GLuint program = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = true;
		if (optimize)
		{
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
		}

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		auto &shaderData = m_VulkanSPIRV;
		shaderData.clear();
		for (auto &&[stage, source] : shaderSources)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto &data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char *)data.data(), size);
			}
			else
			{
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					AK_CORE_ERROR(module.GetErrorMessage());
					AK_CORE_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto &data = shaderData[stage];
					out.write((char *)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto &&[stage, data] : shaderData)
		{
			Reflect(stage, data);
		}
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		auto &shaderData = m_OpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = false;
		if (optimize)
		{
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
		}

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		shaderData.clear();
		m_OpenGLSourceCode.clear();
		for (auto &&[stage, spirv] : m_VulkanSPIRV)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto &data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char *)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_OpenGLSourceCode[stage] = glslCompiler.compile();
				auto &source = m_OpenGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					AK_CORE_ERROR(module.GetErrorMessage());
					AK_CORE_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto &data = shaderData[stage];
					out.write((char *)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	void OpenGLShader::CreateProgram()
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto &&[stage, spirv] : m_OpenGLSPIRV)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			AK_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_FilePath, infoLog.data());

			glDeleteProgram(program);

			for (auto id : shaderIDs)
			{
				glDeleteShader(id);
			}
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID = program;
	}

	void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t> &shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		AK_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_FilePath);
		AK_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
		AK_CORE_TRACE("    {0} resources", resources.sampled_images.size());

		AK_CORE_TRACE("Uniform buffers:");
		for (const auto &resource : resources.uniform_buffers)
		{
			const auto &bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			AK_CORE_TRACE("  {0}", resource.name);
			AK_CORE_TRACE("    Size = {0}", bufferSize);
			AK_CORE_TRACE("    Binding = {0}", binding);
			AK_CORE_TRACE("    Members = {0}", memberCount);
		}
	}

	void OpenGLShader::Bind() const
	{
		// AK_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		// AK_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string &name, int value)
	{
		// AK_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string &name, int *values, uint32_t count)
	{
		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetFloat(const std::string &name, float value)
	{
		// AK_PROFILE_FUNCTION();

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string &name, const glm::vec2 &value)
	{
		// AK_PROFILE_FUNCTION();

		UploadUniformFloat2(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string &name, const glm::vec3 &value)
	{
		// AK_PROFILE_FUNCTION();

		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string &name, const glm::vec4 &value)
	{
		// AK_PROFILE_FUNCTION();

		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string &name, const glm::mat4 &value)
	{
		// AK_PROFILE_FUNCTION();

		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string &name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string &name, int *values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string &name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string &name, const glm::vec2 &value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string &name, const glm::vec3 &value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string &name, const glm::vec4 &value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string &name, const glm::mat3 &matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string &name, const glm::mat4 &matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}