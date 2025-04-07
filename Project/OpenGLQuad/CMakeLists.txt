cmake_minimum_required(VERSION 3.10)
project(OpenGLQuad)

set(CMAKE_CXX_STANDARD 17)

include_directories(include libs/glfw/include)

add_subdirectory(libs/glfw)

file(GLOB SRC_FILES "src/*.cpp" "src/*.c" "*.cpp")

add_executable(OpenGLQuad ${SRC_FILES})

target_link_libraries(OpenGLQuad glfw ${CMAKE_DL_LIBS})
