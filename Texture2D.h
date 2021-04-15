#pragma once
#include <glad.h>
class Texture2D
{
public:
    //Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
    GLuint ID;
    //Texture image dimensions
    unsigned int width, height; // width and height of loaded image in pixels
    //Texture Format
    unsigned int internalFormat; // format of texture object
    unsigned int imageFormat; // format of loaded image
    //Texture configuration
    unsigned int wrapS; // wrapping mode on S axis
    unsigned int wrapT; // wrapping mode on T axis
    unsigned int filterMin; // filtering mode if texture pixels < screen pixels
    unsigned int filterMax; // filtering mode if texture pixels > screen pixels
    //Constructor (sets default texture modes)
    Texture2D();
    //Generates texture from image data
    void generate(unsigned int width, unsigned int height, unsigned char* data);
    //Binds the texture as the current active GL_TEXTURE_2D texture object
    void bind() const;
};

