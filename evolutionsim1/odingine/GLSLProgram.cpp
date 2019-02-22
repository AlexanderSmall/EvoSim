#include "GLSLProgram.h"
#include "Errors.h"

#include <vector>
#include <fstream>
#include <iostream>

namespace odingine {

	GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
	{

	}


	GLSLProgram::~GLSLProgram()
	{
	}

	void GLSLProgram::compileShaders(const std::string & vertexShaderFilePath, const std::string & fragmentShaderFilePath)
	{
		// Vertex and fragment shaders are successfully compiled
		// now time to link them
		// get a program object
		_programID = glCreateProgram();

		// Vertex Shader
		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (_vertexShaderID == 0) {
			fatalError("VertexShader failed to be created");
		}

		//Fragment Shader
		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (_vertexShaderID == 0) {
			fatalError("FragmentShader failed to be created");
		}

		compileShader(vertexShaderFilePath, _vertexShaderID);
		compileShader(fragmentShaderFilePath, _fragmentShaderID);

		GLint compiled{ GL_FALSE };
		glGetShaderiv(_vertexShaderID, GL_COMPILE_STATUS, &compiled);
		if (compiled != GL_TRUE)
		{
			std::cout << "Shader compilation failed:" << /*getShaderLog(shaderID)
				<<*/ std::endl;
			throw std::runtime_error{ "Shader compilation failed" };
		}
	}

	void GLSLProgram::linkShaders()
	{
		// attach out shaders to out program
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		//link our program
		glLinkProgram(_programID);

		// note the dufferent fuction here: glGetProgram* instead of glGEtShader*
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxlength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

			// delete program
			glDeleteProgram(_programID);
			// don't leak shaders
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);


			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shaders failed to link");
		}

		// always detac shaders after successful link
		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);

		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

	}

	void GLSLProgram::addAttribute(const std::string& attributeName)
	{
		glBindAttribLocation(_programID, _numAttributes, attributeName.c_str());
		_numAttributes++;
	}

	GLint GLSLProgram::getUniformLocation(const std::string& uniformName)
	{
		GLint location = glGetUniformLocation(_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX) {
			fatalError("Uniform " + uniformName + " not found in shader");
		}
		return location;
	}

	void GLSLProgram::use()
	{
		glUseProgram(_programID);
		for (int i = 0; i < _numAttributes; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unuse()
	{
		glUseProgram(0);
		for (int i = 0; i < _numAttributes; i++) {
			glDisableVertexAttribArray(i);
		}
	}

	void GLSLProgram::compileShader(const std::string & filepath, GLuint id)
	{
		// read shaders from file
		std::ifstream file(filepath);
		if (file.fail()) {
			perror(filepath.c_str());
			fatalError("Failed to open: " + filepath);
		}

		std::string fileContents = "";
		std::string line;

		while (std::getline(file, line)) {
			fileContents += line + "\n";
		}

		file.close();

		const char* contentsPtr = fileContents.c_str();
		glShaderSource(id, 1, &contentsPtr, nullptr);

		glCompileShader(id);

		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		
		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best
			// Exit with failure
			glDeleteShader(id); // Don't leak the shader

			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader " + filepath + " failed to compile");

		}
		
	}

}
