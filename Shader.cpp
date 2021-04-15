#include "Shader.h"
#include <sstream>

Shader &Shader::use()
{
    glUseProgram(this->ID);
    return *this;
}

bool Shader::compile(const char *vertexSource, const char *fragmentSource){
    
    GLuint vertex, fragment;

    bool success = true;

    //Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);
    success = checkCompileErrors(vertex, "VERTEX");

    //Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);
    success = checkCompileErrors(fragment, "FRAGMENT");


    //Shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    success = checkCompileErrors(ID, "PROGRAM");

    //Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return success;

}

bool Shader::checkCompileErrors(GLuint shader, std::string shaderType){
        bool compiled = true;
        int success;
        char infoLog[1024];
        if (shaderType != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << shaderType << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                compiled = false;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << shaderType << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                compiled = false;
            }
        }
        return compiled;
}
