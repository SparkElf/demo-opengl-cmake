# minimum cmake version
cmake_minimum_required(VERSION 3.22)
# define variety
set(PROJECT_NAME opengldemo)
# set project name
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 23)



# set src dir 
add_subdirectory(src)# 采用这种写法可以方便构建多模块（多src）项目 不推荐使用inclde_directories(src)
# set resource dir
file(COPY resource DESTINATION src)


