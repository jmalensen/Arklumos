
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



# Link the glad dll with the required libraries depending on the system
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	message(STATUS "Glad for Windows")
	# (as we are using Windows with mingw64, we need also opengl32 gdi32 for glad)
	target_link_libraries(${PROJECT_NAME} opengl32 gdi32)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	message(STATUS "Glad for Linux")
	#target_link_libraries(${PROJECT_NAME})
endif()
