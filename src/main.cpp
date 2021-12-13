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
float fov = 45.0f;
float lastX = screenWidth / 2.0f , lastY = screenHeight / 2.0f;
float yawAngle = -90.0f , pitchAngle = 0.0f;//yaw一开始是-90度的，因为0度看向x轴正方向，我们应该看向z轴负方向
Camera camera = Camera(vec3(0.0f , 0.0f , 3.0f) , vec3(0.0f , 0.0f , -1.0f) , vec3(0.0f , 1.0f , 0.0f));//z轴正方向沿屏幕向外
float lastTime=0.0f;
float deltaTime=0.0f;
enum class ShapeEnum {
	Triangle = 0 , Quad = 1 , Cube = 2 , Cubes = 3
};
ShapeEnum curShape = ShapeEnum::Triangle;
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
		curShape = ShapeEnum(key - 320);
	}
	camera.speed=2.5f*deltaTime;
	if (glfwGetKey(window , GLFW_KEY_W) == GLFW_PRESS)
		camera.pos += camera.speed * camera.targetPos;
	if (glfwGetKey(window , GLFW_KEY_S) == GLFW_PRESS)
		camera.pos -= camera.speed * camera.targetPos;
	if (glfwGetKey(window , GLFW_KEY_A) == GLFW_PRESS)
		camera.pos -= normalize(cross(camera.targetPos , camera.up)) * camera.speed;//前向量叉乘上向量得到的是右向量，注意要进行归一化否则不同情况下向右的速度可能会不同。
	if (glfwGetKey(window , GLFW_KEY_D) == GLFW_PRESS)
		camera.pos += normalize(cross(camera.targetPos , camera.up)) * camera.speed;
}
void scroll_callback(GLFWwindow* window , double xoffset , double yoffset) {
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0;
}
bool firstMouse = true;
void mouse_callback(GLFWwindow* window , double xpos , double ypos) {
	if (firstMouse) // 这个bool变量初始时是设定为true的
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
		cout<<"fist"<<endl;
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
	cout<<"pitchAngle: "<<pitchAngle<<" yawAngle: "<<yawAngle<<endl;
	if (pitchAngle > 89.0f)//最多看到头顶的天空，不允许头反向
		pitchAngle = 89.0f;
	if (pitchAngle < -89.0f)
		pitchAngle = -89.0f;

	vec3 front;
	front.x = cos(radians(pitchAngle)) * cos(radians(yawAngle));
	front.y = sin(radians(pitchAngle));
	front.z = cos(radians(pitchAngle)) * sin(radians(yawAngle));
	camera.targetPos = glm::normalize(front);
	cout<<to_string(camera.targetPos)<<endl;
}
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
	glfwSetKeyCallback(window , key_callback);//鼠标键盘事件
	glfwSetScrollCallback(window , scroll_callback);//鼠标滚轮事件
	glfwSetCursorPosCallback(window , mouse_callback);
	while (!glfwWindowShouldClose(window)) {//如果窗口未关闭则执行程序
		//更新全局时钟
		float currentTime= glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;	
		//隐藏鼠标并开启鼠标事件
		glfwSetInputMode(window , GLFW_CURSOR , GLFW_CURSOR_DISABLED);

		//提交事件到后台执行 不提交事件的话glfw会持续阻塞 事件模型为触发入队-提交出队-后台执行
		glfwPollEvents();

		glClearColor(0.2f , 0.3f , 0.3f , 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		model = rotate(model , radians(50.0f) / 60 , vec3(0.5f , 1.0f , 0.0f));
		camera.updateView();
		view = camera.getView();
		projection = perspective(radians(fov) , 1.0f * screenWidth / screenHeight , 0.1f , 100.0f);
		Model* mo = NULL;
		switch (curShape)
		{
		case ShapeEnum::Triangle:
			mo = &triangle;
			break;
		case ShapeEnum::Quad:
			mo = &quad;
			break;
		case ShapeEnum::Cube:
			mo = &cube;
			break;
		case ShapeEnum::Cubes:
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
		if (curShape < ShapeEnum::Cubes) {
			mo->shader->setMVP(model , view , projection);
			mo->draw();
		}

		//最终展示
		glfwSwapBuffers(window);//涉及双缓冲机制
	}
	glfwTerminate();
	return 0;
}
