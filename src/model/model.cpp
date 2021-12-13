#pragma once
#include <glad/glad.h>
#include "shader/shader.cpp"
#include "2D/texture/texture.cpp"
typedef unsigned int uint;

struct Model {
    struct vertex {
        float x , y , z , tx , ty;//3维坐标值和纹理坐标值
    };

    uint vao , vbo , ibo;
    shared_ptr<Shaders>shader;
    shared_ptr<Texture>texture;
    Model() {}
    Model(shared_ptr<Shaders>shader,shared_ptr<Texture>texture){
        this->shader=shader;
        this->texture=texture;
    }
    void useShader(shared_ptr<Shaders>shader) {
        this->shader = shader;
    }
    void useTexture(shared_ptr<Texture>texture) {
        this->texture = texture;
    }
    virtual void draw() {
        if (shader != NULL)
            shader->use();
        if (texture != NULL)
            texture->use();
        //cout<<"model"<<endl;
    }
};