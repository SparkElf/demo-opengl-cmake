#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
using namespace glm;
using namespace std;
enum class SceneEnum {
    Triangle = 0 , Quad = 1 , Cube = 2 , Cubes = 3
};

class Camera {
public:
    vec3 targetPos = vec3() , up = vec3() , pos = vec3();
    mat4 view;
    float speed = 0.5f;
    Camera() {
        cout << "?" << endl;
    }
    Camera(vec3 pos , vec3 targetPos , vec3 up) :pos(pos) , targetPos(targetPos) , up(up) {//target pos 0.0 0.0 0.0异常？？？原因不明
        cout << to_string(view) << endl;
        cout << "in" << endl;
        cout << to_string(pos) << endl;
        cout << to_string(targetPos) << endl;
        cout << to_string(up) << endl;
        cout << to_string(pos + targetPos) << endl;
        view = lookAt(pos , pos + targetPos , up);
        cout << to_string(view) << endl;
    }
    void updateView() {
        //这样设计可以在移动时平移视线 实现头、脚分离
        view = lookAt(pos , pos + targetPos , up);
    }
    mat4& getView() {
        return view;
    }

    bool firstMouse = true;
    float fov = 45.0f;
    float lastX , lastY;
    float yawAngle = -90.0f , pitchAngle = 0.0f;//yaw一开始是-90度的，因为0度看向x轴正方向，我们应该看向z轴负方向
    float lastTime = 0.0f;
    float deltaTime = 0.0f;

    SceneEnum curScene = SceneEnum::Triangle;


    void scroll_callback(GLFWwindow* window , double xoffset , double yoffset) {
        fov -= (float)yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0;
    }
    void mouse_callback(GLFWwindow* window , double xpos , double ypos) {
        if (firstMouse) // 这个bool变量初始时是设定为true的
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;//原点在左下角
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.05f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yawAngle += xoffset;//相机转角
        pitchAngle += yoffset;

        if (pitchAngle > 89.0f)//最多看到头顶的天空，不允许头反向
            pitchAngle = 89.0f;
        if (pitchAngle < -89.0f)
            pitchAngle = -89.0f;

        vec3 front;
        front.x = cos(radians(pitchAngle)) * cos(radians(yawAngle));
        front.y = sin(radians(pitchAngle));
        front.z = cos(radians(pitchAngle)) * sin(radians(yawAngle));
        targetPos = glm::normalize(front);
        cout << to_string(targetPos) << endl;
    }

    void key_callback(GLFWwindow* window , int key , int scancode , int action , int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window , true);
        if (key == GLFW_KEY_P && action == GLFW_PRESS) {
            int mode[2];
            glGetIntegerv(GL_POLYGON_MODE , mode);
            switch (mode[0]) {
            case GL_FILL:
                glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);
                break;
            case GL_LINE:
                glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
                break;
            }
        }
        if (key >= 320 && key <= 323 && action == GLFW_PRESS) {
            curScene = SceneEnum(key - 320);
        }
        speed = 2.5f * deltaTime;
        if (glfwGetKey(window , GLFW_KEY_W) == GLFW_PRESS)
            pos += speed * targetPos;
        if (glfwGetKey(window , GLFW_KEY_S) == GLFW_PRESS)
            pos -= speed * targetPos;
        if (glfwGetKey(window , GLFW_KEY_A) == GLFW_PRESS)
            pos -= normalize(cross(targetPos , up)) * speed;//前向量叉乘上向量得到的是右向量，注意要进行归一化否则不同情况下向右的速度可能会不同。
        if (glfwGetKey(window , GLFW_KEY_D) == GLFW_PRESS)
            pos += normalize(cross(targetPos , up)) * speed;
    }
    void updateTime() {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
    }
};