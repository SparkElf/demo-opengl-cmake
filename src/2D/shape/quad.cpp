#pragma once
#include <glad/glad.h>
#include "shader/shader.cpp"
#include "2D/texture/texture.cpp"
typedef unsigned int uint;

class Quad {
    Shaders* shader;
    Texture* texture;
    uint vao , vbo , ibo;
    struct vertex {
        float x , y , z,tx,ty;//后两个是纹理坐标
    };
public:
    Quad() {
        
        vertex vertexes[] = {
            vertex(0.5f,0.5f,0.0f,1.0f,1.0f),//右上角
            vertex(0.5f,-0.5f,0.0f,1.0f,0.0f),//右下角
            vertex(-0.5f,-0.5f,0.0f,0.0f,0.0f),//左下角
            vertex(-0.5f,0.5f,0.0f,0.0f,1.0f)//左上角
        };

        uint indices[]={
            0,1,3,
            1,2,3
        };
        glGenVertexArrays(1 , &vao);
        glGenBuffers(1 , &vbo);
        glGenBuffers(1,&ibo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER , vbo);
        glBufferData(GL_ARRAY_BUFFER , sizeof(vertexes) , vertexes , GL_STATIC_DRAW);

        glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , sizeof(vertex) , (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1 , 2 , GL_FLOAT , GL_FALSE , sizeof(vertex) , (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

        
    }
    void useShader(Shaders* shader) {
        this->shader = shader;
    }
    void useTexture(Texture* texture){
        this->texture=texture;
    }
    void draw() {
         if(shader!=NULL)
            shader->use();
        if(texture!=NULL)
            texture->use();
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        //glBindVertexArray(0);//解绑定vao
    }
};