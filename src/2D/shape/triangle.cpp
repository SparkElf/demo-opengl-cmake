#pragma once
#include <glad/glad.h>
#include "shader/shader.cpp"
#include "2D/texture/texture.cpp"
typedef unsigned int uint;

class Triangle{
    Shaders* shader;
    Texture* texture;
    uint vao,vbo,ibo;
    struct vertex{
        float x,y,z,tx,ty;//3维坐标值和纹理坐标值
    };
public:
    Triangle(){
        vertex vertexes[]={
            vertex(-0.5f,-0.5f,0.0f,0.0f,0.0f),
            vertex(0.5f,-0.5f,0.0f,1.0f,0.0f),
            vertex(0.0f,0.5f,0.0f,0.5f,1.0f)
        };
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo);  
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
        glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , sizeof(vertex) , NULL);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1 , 2 , GL_FLOAT , GL_FALSE , sizeof(vertex) , (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    void useShader(Shaders *shader){
        this->shader=shader;
    }
    void useTexture(Texture* texture){
        this->texture=texture;
    }
    void draw(){
        if(shader!=NULL)
            shader->use();
        if(texture!=NULL)
            texture->use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,3);
    }
};