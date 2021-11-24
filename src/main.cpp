#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <format>
#include <filesystem>
#include "shader/shader.cpp"
#include "2D/shape/triangle.cpp"
#include "2D/shape/quad.cpp"
using namespace std;
namespace fs = std::filesystem;

enum class ShapeEnum{
	Triangle=0,Quad=1,Circle=2
};
ShapeEnum curShape=ShapeEnum::Triangle;
void key_callback( GLFWwindow * window, int key, int scancode , int action, int mods)
{
    if (key==GLFW_KEY_ESCAPE&& action == GLFW_PRESS)
		glfwSetWindowShouldClose(window , true);
	if (key==GLFW_KEY_P && action== GLFW_PRESS){
		int mode[2];
		glGetIntegerv(GL_POLYGON_MODE,mode);
		switch (mode[0]){
		case GL_FILL:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case GL_LINE:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}
	}
	if (key>=320&&key<=322 && action== GLFW_PRESS){
		curShape=ShapeEnum(key-320);
	}
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
	} catch (Exception& e) {}

	Texture texture=Texture();
	try{
		texture.load("resource/texture/wall.jpg");
	}
	catch(Exception &e){}
	
	
	Triangle triangle = Triangle();
	triangle.useShader(&shaders);
	triangle.useTexture(&texture);

	Quad quad=Quad();
	quad.useShader(&shaders);
	quad.useTexture(&texture);
	
	while (!glfwWindowShouldClose(window)) {//如果窗口未关闭则执行程序
		//先处理事件
		//触发并提交事件到队列
		glfwSetKeyCallback(window,key_callback);
		//提交事件到后台执行 不提交事件的话glfw会持续阻塞 事件模型为触发入队-提交出队-后台执行
		glfwPollEvents();

		glClearColor(0.2f , 0.3f , 0.3f , 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		switch (curShape)
		{
		case ShapeEnum::Triangle:
			triangle.draw();
			break;
		case ShapeEnum::Quad:
			quad.draw();
		default:
			break;
		}

		//最终展示
		glfwSwapBuffers(window);//涉及双缓冲机制
	}
	glfwTerminate();
	return 0;
}
