# merge codes

file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

if(MSVC)
# 自定义编译规则
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_FLAGS "/NODEFAULTLIB:MSVCRT")
set(CMAKE_C_FLAGS /source-charset:utf-8)
endif()

# libraries

# 必须在src中引入依赖而不能在项目根目录引入依赖，否则无法找到头文件，原因未知

set(PKG_DIR D:/software/cmakeCache)

# GLFW

set(GLFW_LIB ${PKG_DIR}/glfw-3.3.5/lib-vc2019/glfw3.lib)
set(GLFW_INCLUDE ${PKG_DIR}/glfw-3.3.5/include)

# GLAD

add_subdirectory(${PKG_DIR}/glad/ build/glad)

# stb_image

add_subdirectory(${PKG_DIR}/stb_image/ build/stb_image)

# GLM

set(GLM_INCLUDE ${PKG_DIR}/glm-0.9.8.5/glm)

# merge includes

list(APPEND INCLUDES ./)
list(APPEND INCLUDES ${GLFW_INCLUDE})
list(APPEND INCLUDES ${GLM_INCLUDE})
include_directories(${INCLUDES})

# merge libs

list(APPEND LIBS ${GLFW_LIB})
list(APPEND LIBS glad)
list(APPEND LIBS stb_image)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})