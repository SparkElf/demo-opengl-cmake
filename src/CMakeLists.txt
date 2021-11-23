
# merge codes
file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

if(MSVC)
	# 自定义编译规则
	set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_FLAGS "/NODEFAULTLIB:MSVCRT")

endif()


# libraries
# 必须在src中引入依赖而不能在项目根目录引入依赖，否则无法找到头文件，原因未知
set(PKG_DIR D:/software/cmakeCache)
# GLFW
set(GLFW_LIB ${PKG_DIR}/glfw-3.3.5/lib-vc2019/glfw3.lib)
set(GLFW_INCLUDE ${PKG_DIR}/glfw-3.3.5/include)
# GLAD
add_subdirectory(${PKG_DIR}/glad/ build)


# merge includes
list(APPEND INCLUDES ${GLFW_INCLUDE})
include_directories(${INCLUDES})

# merge libs
list(APPEND LIBS ${GLFW_LIB})
list(APPEND LIBS glad)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})

