#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;


void triggerInputEvents(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(){
	glfwInit();
	GLFWwindow *window=glfwCreateWindow(640,480,"Demo",NULL,NULL);
	glfwMakeContextCurrent(window);
	//初始化glad，将OpenGL函数载入glfw进程
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	while(! glfwWindowShouldClose (window)){//如果窗口未关闭则执行程序
		//触发并提交事件到队列
		triggerInputEvents(window);
	 	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		//提交事件到后台执行 不提交事件的话glfw会持续阻塞 事件模型为触发入队-提交出队-后台执行
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
