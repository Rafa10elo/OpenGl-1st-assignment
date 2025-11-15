cmake_minimum_required(VERSION 3.10)
project(jesus)
set(CMAKE_CXX_STANDARD 17)

set(GLFW_DIR "B:/OpenGl/glfw64")
include_directories(${GLFW_DIR}/include)
link_directories(${GLFW_DIR}/lib-mingw-w64)

set(GLEW_DIR "B:/OpenGl/glew")
include_directories(${GLEW_DIR}/include)
link_directories(${GLEW_DIR}/lib/Release/x64)

add_executable(jesus main.cpp)


target_link_libraries(jesus glfw3 glew32 opengl32)
