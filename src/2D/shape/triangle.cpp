#pragma once
#include <glad/glad.h>
#include "shader/shader.cpp"
#include "2D/texture/texture.cpp"
#include "model/model.cpp"
typedef unsigned int uint;

struct Triangle:public Model{
    Triangle(){
        init();
    }
    Triangle(shared_ptr<Shaders> shader,shared_ptr<Texture>texture):Model(shader,texture){
        init();
    }

    void draw(){
        Model::draw();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,3);
        //cout<<"triangle"<<endl;
    }
    private:
    void init(){
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
};