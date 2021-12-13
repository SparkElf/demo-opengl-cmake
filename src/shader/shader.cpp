#pragma once
#include <glad/glad.h>
#include <fstream>
#include <format>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "util/exception.cpp"
using namespace std;
using namespace glm;
typedef unsigned int uint;

//着色器管线，包含多个shader
class Shaders{
    public:
    uint fd;
    mat4 model,view,projection;
    Shaders(){
        fd=glCreateProgram();
    }
    void use(){
        glUseProgram(fd);
    }
    void addShader(const char*path,int type){
        std::ifstream file(path);
        if(file.fail())
            throw(Exception(format("shader {} not found",path),true));
    
        string str;
        uint shader_fd;

        //转换成字符串
        str=string(istreambuf_iterator<char>(file) ,istreambuf_iterator<char>());
      
        file.close();

        shader_fd=glCreateShader(type);

        auto s=str.c_str();
        glShaderSource(shader_fd,1,&s,NULL);
        glCompileShader(shader_fd);
    
        int success;
        
        glGetProgramiv(shader_fd,GL_COMPILE_STATUS,&success);
        if(!success){
            char infoLog[512]={0};
            glGetProgramInfoLog(shader_fd, 512, NULL, infoLog);
            cout<<"shader "<<path<<" compile failed:"<<endl<<infoLog<<endl;
            throw(Exception());
        }

        glAttachShader(fd,shader_fd);
        glLinkProgram(fd);
    
        success=0;
        glGetProgramiv(fd,GL_LINK_STATUS,&success);
        if(!success){
            char infoLog[512]={0};
            glGetProgramInfoLog(fd, 512, NULL, infoLog);
            cout<<"shader "<<path<<" link failed:"<<endl<<infoLog<<endl;
            throw(Exception());
        }
            
        glDeleteShader(shader_fd);
    }
    void setModel(mat4 &model){
        use();
        glUniformMatrix4fv(glGetUniformLocation(fd,"model"), 1, GL_FALSE, value_ptr(model));
        this->model=model;
    }
    void setView(mat4 &view){
        use();
        glUniformMatrix4fv(glGetUniformLocation(fd,"view"), 1, GL_FALSE, value_ptr(view));
        this->view=view;
    }
    void setProjection(mat4 &projection){
        use();
        glUniformMatrix4fv(glGetUniformLocation(fd,"projection"), 1, GL_FALSE, value_ptr(projection));
        this->projection=projection;
    }
    void setBool(const char* name,bool &value){
        use();
        glUniform1i(glGetUniformLocation(fd,name),value);
    }
    void setMVP(mat4&model,mat4&view,mat4&projection){
        use();
        glUniformMatrix4fv(glGetUniformLocation(fd,"model"), 1, GL_FALSE, value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(fd,"view"), 1, GL_FALSE, value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(fd,"projection"), 1, GL_FALSE, value_ptr(projection));
        this->model=model;
        this->view=view;
        this->projection=projection;
    }
};