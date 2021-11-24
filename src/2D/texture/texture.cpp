#pragma once
#include <string>
#include <stb_image.h>
#include <glad/glad.h>
#include <format>
#include "util/exception.cpp"
typedef unsigned int uint;
struct Texture {
public:
    int width , height , channel;
    uint fd;
public:
    Texture(){}
    Texture(const char* path) {
        load(path);
    }
    void use() {
        glBindTexture(GL_TEXTURE_2D , fd);
    }
    void load(const char* path) {
        // 1. 使用 stbi 从 file_name 加载图像数据
        stbi_set_flip_vertically_on_load(true);//将图片坐标原点调整为左下角
        auto img_data = stbi_load(path , &width , &height , &channel , 0);
        
        if(img_data==NULL)
            cout<<format("texture {} load failed",path)<<endl;
        
        // 2. 生成 texture object, 请使用tex_id
        glGenTextures(1 , &fd);
        // 3. 绑定当前纹理
        glBindTexture(GL_TEXTURE_2D , fd);
        // 4. 设置纹理参数 (提示: 过滤模式请使用 GL_NEAREST)
        //横向重复
        glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT);
        //纵向重复
        glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR_MIPMAP_LINEAR);
        // 5. 拷贝图像数据到显存 (提示: 通道数取决于channel)
        glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGB , width , height , 0 , GL_RGB , GL_UNSIGNED_BYTE , img_data);
        // 6. 生成 多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);
        // 7. 释放内存
        stbi_image_free(img_data);
    }
};