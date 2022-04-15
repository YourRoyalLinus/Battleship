#pragma once
#include <chrono>
#include <filesystem>
#include <string>
#include <map>
#include "Shader.h"
#include "Texture2D.h"
//#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

class ResourceManager
{
public:
    //Resource storage
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture2D> textures;
    //Loads (and generates) a shader program from file loading vertex & fragment 
    static Shader loadShader(const char* vShaderFile, const char* fShaderFile, std::string name);
    //Retrieves a stored sader
    static Shader getShader(std::string name);
    //Loads (and generates) a texture from file
    static Texture2D loadTexture(const char* file, unsigned int internalFormat, unsigned int imageFormat, std::string name);
    //Retrieves a stored texture
    static Texture2D getTexture(std::string name);

    static void hotReload();
    //Properly de-allocates all loaded resources
    static void clear();
private:
    //Singleton!
    ResourceManager() {}
    //Loads and generates a shader from file
    static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile);
    //Loads a single texture from file
    static Texture2D loadTextureFromFile(const char* file, unsigned int internalFormat, unsigned int imageFormat);
    //maps shader file to time last changed used for hot reloading
    static std::map<std::string, std::filesystem::file_time_type> shaderChangeTimes;
};


