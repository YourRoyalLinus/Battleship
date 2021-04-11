#include "ResourceManager.h"
#include <sstream>
#include <stb_image.h>

//init static members
std::map<std::string, Texture2D> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;


Shader ResourceManager::loadShader(const char *vShaderFile, const char *fShaderFile, std::string name)
{
    shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
    return shaders[name];
}

Shader ResourceManager::getShader(std::string name)
{
    return shaders[name];
}

Texture2D ResourceManager::loadTexture(const char *file, bool alpha, std::string name)
{
    textures[name] = loadTextureFromFile(file, alpha);
    return textures[name];
}

Texture2D ResourceManager::getTexture(std::string name)
{
    return textures[name];
}

void ResourceManager::clear()
{
    //Delete all shaders	
    for (auto iter : shaders)
        glDeleteProgram(iter.second.ID);
    //Delete all textures
    for (auto iter : textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile)
{
    //Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        //Open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);

        std::stringstream vShaderStream, fShaderStream;
        //Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        //Close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        //Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::exception e)
    {
        std::cout << "RESOURCE MANAGER FAILED TO READ SHADER FILES!" << std::endl;
    }
    //NOTE: When compiling shaders, OpenGL expects a pointer to an lvalue c-style string; so I need to convert these to c-style string before passing it to compile
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();

    //Noww create shader object from source code
    Shader shader;
    shader.compile(vShaderCode, fShaderCode);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char *file, bool alpha)
{
    //Create texture object
    Texture2D texture;
    //I'm assuming images are RGBA by default, if they aren't change that!
    if (!alpha){
        texture.Internal_Format = GL_RGB;
        texture.Image_Format = GL_RGB;
    }
    //Load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

    //Generate texture
    texture.Generate(width, height, data);

    //Finally free image data
    stbi_image_free(data);

    return texture;
}
