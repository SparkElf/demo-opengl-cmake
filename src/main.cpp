#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <format>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "shader/shader.cpp"
#include "2D/shape/triangle.cpp"
#include "2D/shape/quad.cpp"
#include "3D/shape/cube.cpp"
#include "3D/camera/camera.cpp"
using namespace std;
using namespace glm;
namespace fs = std::filesystem;

constexpr int screenWidth = 640 , screenHeight = 480;

Camera camera = Camera(vec3(0.0f , 0.0f , 3.0f) , vec3(0.0f , 0.0f , -1.0f) , vec3(0.0f , 1.0f , 0.0f));//z轴正方向沿屏幕向外


int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(screenWidth , screenHeight , "Demo" , NULL , NULL);
	glfwMakeContextCurrent(window);
	//初始化glad，将OpenGL函数载入glfw进程
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//设置当前进程的工作目录
	fs::current_path("../");
	//初始化shader
	auto shaders = make_shared<Shaders>();
	try {
		shaders->addShader("resource/shader/base.vert" , GL_VERTEX_SHADER);
		shaders->addShader("resource/shader/base.frag" , GL_FRAGMENT_SHADER);

	} catch (Exception& e) {
	}
	shared_ptr texture = make_shared<Texture>();
	try {
		texture->load("resource/texture/wall.jpg");
	} catch (Exception& e) {
	}

	Triangle triangle = Triangle(shaders , texture);
	Quad quad = Quad(shaders , texture);
	Cube cube = Cube(shaders , texture);

	//创建矩阵
	mat4 model = mat4(1.0f);
	mat4 view;
	view = translate(view , vec3(0.0f , 0.0f , -3.0f));
	mat4 projection;
	projection = perspective(radians(45.0f) , 1.0f * screenWidth / screenHeight , 0.1f , 100.0f);

	//初始化shader
	triangle.shader->setMVP(model , view , projection);
	quad.shader->setMVP(model , view , projection);
	cube.shader->setMVP(model , view , projection);
	//cubes
	vec3 translations[] = {
		vec3(0.0f,  0.0f,  0.0f),
		vec3(2.0f,  5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(2.4f, -0.4f, -3.5f),
		vec3(-1.7f,  3.0f, -7.5f),
		vec3(1.3f, -2.0f, -2.5f),
		vec3(1.5f,  2.0f, -2.5f),
		vec3(1.5f,  0.2f, -1.5f),
		vec3(-1.3f,  1.0f, -1.5f)
	};
	Cube cubes[10];
	for (int i = 0;i < 10;i++) {
		auto s = make_shared<Shaders>();
		try {
			s->addShader("resource/shader/base.vert" , GL_VERTEX_SHADER);
			s->addShader("resource/shader/base.frag" , GL_FRAGMENT_SHADER);
		} catch (Exception& e) {
		}
		auto t = make_shared<Texture>();
		try {
			t->load("resource/texture/wall.jpg");
		} catch (Exception& e) {
		}
		mat4 m = mat4();
		m = translate(m , translations[i]);
		m = rotate(m , glm::radians(20.0f * (i + 1)) , glm::vec3(1.0f , 0.3f , 0.5f));
		cubes[i].useShader(s);
		cubes[i].useTexture(t);
		cubes[i].shader->setMVP(m , view , projection);
	}
	//开启深度测试
	glEnable(GL_DEPTH_TEST);
	//触发并提交事件到队列
	glfwSetKeyCallback(window ,[](GLFWwindow* window , int key , int scancode , int action , int mods){ camera.key_callback(window,key,scancode,action,mods);});//鼠标键盘事件
	glfwSetScrollCallback(window ,[](GLFWwindow* window , double xoffset , double yoffset){ camera.scroll_callback(window,xoffset,yoffset);});//鼠标滚轮事件
	glfwSetCursorPosCallback(window ,[](GLFWwindow* window , double xpos , double ypos){ camera.mouse_callback(window,xpos,ypos);});
	while (!glfwWindowShouldClose(window)) {//如果窗口未关闭则执行程序
		//更新全局时钟
		camera.updateTime();
		//隐藏鼠标并开启鼠标事件
		glfwSetInputMode(window , GLFW_CURSOR , GLFW_CURSOR_DISABLED);

		//提交事件到后台执行 不提交事件的话glfw会持续阻塞 事件模型为触发入队-提交出队-后台执行
		glfwPollEvents();

		glClearColor(0.2f , 0.3f , 0.3f , 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		model = rotate(model , radians(50.0f) / 60 , vec3(0.5f , 1.0f , 0.0f));
		camera.updateView();
		view = camera.getView();
		projection = perspective(radians(camera.fov) , 1.0f * screenWidth / screenHeight , 0.1f , 100.0f);
		Model* mo = NULL;
		switch (camera.curScene)
		{
		case SceneEnum::Triangle:
			mo = &triangle;
			break;
		case SceneEnum::Quad:
			mo = &quad;
			break;
		case SceneEnum::Cube:
			mo = &cube;
			break;
		case SceneEnum::Cubes:
			for (int i = 0; i < 10; i++)
			{
				mat4 m = rotate(cubes[i].shader->model , radians(50.0f) / 60.0f , vec3(1.0f , 0.3f , 0.5f));
				cubes[i].shader->setMVP(m , view , projection);
				cubes[i].draw();
			}
			break;
		default:
			break;
		}
		if (camera.curScene < SceneEnum::Cubes) {
			mo->shader->setMVP(model , view , projection);
			mo->draw();
		}

		//最终展示
		glfwSwapBuffers(window);//涉及双缓冲机制
	}
	glfwTerminate();
	return 0;
}
