#pragma once
#include <glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>

 
class Shader
{
public:

	GLuint ID;

	Shader() { }
	//Activate this shader
	Shader& use();

	// compiles the shader from given source code
    void compile(const char *vertexSource, const char *fragmentSource); 

	/* UTILITY FUNCTIONS FOR UNIFORMS */
    //Template who??
	void setUniformBool(const std::string& name, bool value) const {
		GLint boolLocation = glGetUniformLocation(ID, name.c_str());
		glUniform1i(boolLocation, (int)value);
	}

	void setUniformInt(const std::string& name, const int value) const {
		GLint intLocation = glGetUniformLocation(ID, name.c_str());
		glUniform1i(intLocation, value);
	}

	void setUniformFloat(const std::string& name, float value) const {
		GLint floatLocation = glGetUniformLocation(ID, name.c_str());
		glUniform1f(floatLocation, value);
	}
    void setVec2(const std::string& name, const glm::vec2& value) const{ glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); }
    void setVec2(const std::string& name, float x, float y) const { glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); }
    void setVec3(const std::string& name, const glm::vec3& value) const{ glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); }
    void setVec3(const std::string& name, float x, float y, float z) const { glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); }
    void setVec4(const std::string& name, const glm::vec4& value) const { glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); }
    void setVec4(const std::string& name, float x, float y, float z, float w) { glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); }
    void setMat2(const std::string& name, const glm::mat2& mat) const{ glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }
    void setMat3(const std::string& name, const glm::mat3& mat) const { glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }
    void setMat4(const std::string& name, const glm::mat4& mat) const { glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }


private:
	//Utility function for getting any errors compiling the shaders
	void checkCompileErrors(GLuint shader, std::string shaderType);

};

