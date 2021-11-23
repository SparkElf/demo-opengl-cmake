#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <format>
#include <filesystem>
#include "shader/shader.cpp"
#include "2D/triangle.cpp"
using namespace std;
namespace fs = std::filesystem;

void triggerInputEvents(GLFWwindow* window)
{
	if (glfwGetKey(window , GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window , true);
}

int main() {

	glfwInit();
	GLFWwindow* window = glfwCreateWindow(640 , 480 , "Demo" , NULL , NULL);
	glfwMakeContextCurrent(window);
	//初始化glad，将OpenGL函数载入glfw进程
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//设置当前进程的工作目录
	fs::current_path("../");
	//初始化shader
	Shaders shaders = Shaders();
	try {
		shaders.addShader("resource/shader/base.vert" , GL_VERTEX_SHADER);
		shaders.addShader("resource/shader/base.frag" , GL_FRAGMENT_SHADER);
	} catch (Exception& e) {
	}

	Triangle triangle = Triangle();
	triangle.useShader(&shaders);

	
	while (!glfwWindowShouldClose(window)) {//如果窗口未关闭则执行程序
		//先处理事件
		//触发并提交事件到队列
		triggerInputEvents(window);
		//提交事件到后台执行 不提交事件的话glfw会持续阻塞 事件模型为触发入队-提交出队-后台执行
		glfwPollEvents();

		glClearColor(0.2f , 0.3f , 0.3f , 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		triangle.draw();

		//最终展示
		glfwSwapBuffers(window);//涉及双缓冲机制
	}
	glfwTerminate();
	return 0;
}
