cmake_minimum_required(VERSION 3.0)
project(firstprogram)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)

set(ALL_LIBS
  ${OPENGL_LIBRARY}
  glut
  GLEW
  glfw
)

include_directories(inc)

file(GLOB SOURCES "./src/*.cpp")

add_executable(firstprogram main.cpp ${SOURCES})

target_link_libraries(firstprogram
  ${ALL_LIBS}
)

add_custom_target(run
  COMMAND firstprogram
  DEPENDS firstprogram
  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})

