
# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.16)

# Set the project name
set(PROJECT_NAME stb_image)
project(${PROJECT_NAME})

# Add the source files to the project
add_library(${PROJECT_NAME} STATIC
			"${CMAKE_CURRENT_SOURCE_DIR}/stb_image.cpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/stb_image.h"
)