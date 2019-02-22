#pragma once

#include <GL/glew.h>

#include <string>

namespace odingine {

	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		void linkShaders();

		void addAttribute(const std::string& attributeName);

		GLint getUniformLocation(const std::string& uniformName);

		GLuint getProgramID() { return _programID; }

		void use();
		void unuse();
	private:
		int _numAttributes;

		void compileShader(const std::string& filepath, GLuint id);

		GLuint _programID;
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;

	};

}

