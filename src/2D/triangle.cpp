#pragma once
#include <glad/glad.h>
#include "../shader/shader.cpp"
typedef unsigned int uint;
class Triangle{
    Shaders* shader;
    uint vao,vbo,ibo;
public:
    Triangle(){
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
        };
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);  
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 0 , NULL);
        glEnableVertexAttribArray(0);
    }
    void useShader(Shaders *shader){
        this->shader=shader;
    }
    void draw(){
        if(this->shader!=NULL)
            shader->use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,3);
    }
};