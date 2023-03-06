
# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.16)

# Set the project name
set(PROJECT_NAME entt)
project(${PROJECT_NAME})

# Add the source files to the project
add_library(${PROJECT_NAME} STATIC
			"${CMAKE_CURRENT_SOURCE_DIR}/include/entt.hpp"
)
set_target_properties(${PROJECT_NAME} PROPERTIES LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)