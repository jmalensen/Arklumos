#include "akpch.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Arklumos
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

	/*
		Constructor for the OpenGLShader with a filepath

		First, the function reads the contents of the file using the ReadFile function. Then, it calls the PreProcess function which preprocesses the shader source code and separates the shader code into individual shader sources (vertex, fragment, geometry, etc.) that can be compiled separately. Finally, the Compile function is called to compile the shader sources.

		After compiling the shader sources, the constructor extracts the name of the shader from the file path by finding the last occurrence of a slash (/ or \) character to determine the beginning of the filename, and the last occurrence of a dot (.) character to determine the end of the filename extension. The substring of the file path between these two indices is set as the name of the shader using the substr function
	*/
	OpenGLShader::OpenGLShader(const std::string &filepath)
	{
		// AK_PROFILE_FUNCTION();

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

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
		Compile(sources);
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
			AK_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			// Start of shader code after shader type declaration line (This is done by finding the position of the first non-newline character after the end of the #type line)
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			AK_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			// Start of next shader type declaration line
			// Find the next #type line in the shader source code, starting at the position of the first non-newline character after the end of the previous shader code block.
			// If there is no next #type line, the shader code block extends to the end of the shader source code.
			pos = source.find(typeToken, nextLinePos);

			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);

			// Repeat until there are no more #type lines in the shader source code.
		}

		return shaderSources;
	}

	// Compiles and attaches shader objects to the program
	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string> &shaderSources)
	{
		// AK_PROFILE_FUNCTION();

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
		AK_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto &kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string &source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar *sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				AK_CORE_ERROR("{0}", infoLog.data());
				AK_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		//
		m_RendererID = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs)
			{
				glDeleteShader(id);
			}

			AK_CORE_ERROR("{0}", infoLog.data());
			AK_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
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