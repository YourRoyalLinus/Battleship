#include "SpriteRenderer.h"
#include <glm/ext.hpp>

SpriteRenderer::SpriteRenderer(Shader &shader){
    this->shader = shader;
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer(){
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::initRenderData(){
    //Configure VAO/VBO
    unsigned int VBO;
    float vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    //Generate VAO & Generate/Bind VBO
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec4 color, glm::vec2 pointOfRotation, Texture2D& normalMap){
    //Prepare transformations
    this->shader.use();
    glm::mat4 model = glm::mat4(1.0f);

    //NOTE!!!!: Matrix mutiplication is not communicative!!!!!
    //Matrix multiplication occurs right to left; here we are actually SCALING FIRST, THEN ROTATING, AND FINALLY TRANSLATING.
    //This is important because if we were to do it in the wrong order it wouldn't give the expected result
    //If we were to do them in order in the code as scale then rotate then translate we would get a totally different matrix.

    model = glm::translate(model, glm::vec3(position, 0.0f));  //translate to position
    
    //rotate
    //NOTE: the origin of the Sprite is it's top-left corner. In order to rotate about it's center; (which is what we want by default) 
    //first move to where it's center is, then rotate, and finally move it back.
    //Also, because scaling happens first, we must take it's updated size into account when moving to it's center hence: (0.5 * size.x ... )

    if (pointOfRotation == glm::vec2()) {
        //Caller didn't specify point of rotation so rotate about center of sprite
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    }
    else {
        //Caller did speciy point of rotation
        model = glm::translate(model, glm::vec3(pointOfRotation.x, pointOfRotation.y, 0.0f)); 
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
        model = glm::translate(model, glm::vec3(-pointOfRotation.x, -pointOfRotation.y, 0.0f));
    }
    
    //scale
    model = glm::scale(model, glm::vec3(size, 1.0f)); 
  
    this->shader.setMat4("model", model);
    this->shader.setVec4("spriteColor", color);
  
    glActiveTexture(GL_TEXTURE0);
    texture.bind();
    if (normalMap.ID != 0) {
        glActiveTexture(GL_TEXTURE1);
        normalMap.bind();
        glActiveTexture(GL_TEXTURE0);
    }
     
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    

}  
