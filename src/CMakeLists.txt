add_executable(${PROJECT_NAME} main.cpp)

if(MSVC)
	# 自定义编译规则
	set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_FLAGS "/NODEFAULTLIB:MSVCRT")
endif()

# libraries
# 必须在src中引入依赖而不能在项目根目录引入依赖，否则无法找到头文件，原因未知
set(PKG_DIR D:/software/cmakeCache)
#[[ GLEW
set(GLEW_LIB  ${PKG_DIR}/glew-2.1.0/lib/Release/x64/glew32s.lib)
set(GLEW_INCLUDE ${PKG_DIR}/glew-2.1.0/include)
add_definitions(-D GLEW_STATIC)# 如果使用的是静态库需要添加宏定义]]
# GLFW
set(GLFW_LIB ${PKG_DIR}/glfw-3.3.5/lib-vc2019/glfw3.lib)
set(GLFW_INCLUDE ${PKG_DIR}/glfw-3.3.5/include)
#[[ OPENGL
include(FindOpenGL)
find_package( OpenGL REQUIRED)]]
# GLAD
add_subdirectory(${PKG_DIR}/glad/ build)


# merge includes
#list(APPEND INCLUDES ${GLEW_INCLUDE})
list(APPEND INCLUDES ${GLFW_INCLUDE})
include_directories(${INCLUDES})
# merge libs
#list(APPEND LIBS ${GLEW_LIB})
#list(APPEND LIBS ${OPENGL_gl_LIBRARY})
list(APPEND LIBS ${GLFW_LIB})
list(APPEND LIBS glad)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})
