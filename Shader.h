#pragma once
#include <glad.h>
#include <string>
#include <iostream>
#include <fstream>
class Shader
{
public:
	unsigned int ID;

	//Construct a shader program from vertex and fragment source file paths
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	//Activate this shader
	void use() { glUseProgram(ID); }
	//Copying a shader doesn't make sense, so delete those constructors
	//Shader(const Shader&) = delete;
	//Shader& operator=(const Shader&) = delete;
	/*Shader(const Shader&&) = delete;
	Shader& operator=(const Shader&&) = delete;*/


	/* UTILITY FUNCTIONS FOR UNIFORMS */
	/*void setUniformBool(const std::string& name, bool value) const {
		GLint boolLocation = glGetUniformLocation(ID, name.c_str());
		glUniform1i(boolLocation, (int)value);
	}

	void setUniformInt(const std::string& name, const int value) const {
		GLint intLocation = glGetUniformLocation(ID, name.c_str());
		glUniform1i(intLocation, value);
	}

	void setUniformFloat(const std::string& name, float value) const {
		GLint floatLocation = glGetUniformLocation(ID, name.c_str());
		glUniform1i(floatLocation, value);
	}*/

	template <typename T>
	void setUniform(const std::string& name, T value) {
		GLint location = glGetUniformLocation(ID, name.c_str());
		glUniform1i(location, value);
	}







private:
	void compileShaders();
	//Utility function for getting any errors compiling the shaders
	void checkCompileErrors(GLuint shader, std::string shaderType);
	//Shader file input streams
	std::ifstream vShaderFile, fShaderFile;
	//Actual Shader Source code
	std::string vertexShaderCode, fragmentShaderCode;

};

