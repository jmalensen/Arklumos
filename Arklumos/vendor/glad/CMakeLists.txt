
# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.16)

# Set the project name
set(PROJECT_NAME glad)
project(${PROJECT_NAME})

# Add the source files to the project
add_library(${PROJECT_NAME} STATIC 
            "src/glad.c"
            "include/glad/glad.h"
            "include/KHR/khrplatform.h"
            )

target_link_libraries(${PROJECT_NAME} opengl32 gdi32)
