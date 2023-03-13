# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.24)

project(${PROJECT_NAME} LANGUAGES CXX)
message("> START Arklumos library")

# Find the files for the Arklumos library
# (should I target each file one by one ?)
file(GLOB_RECURSE SRC_FILES
  "${CMAKE_SOURCE_DIR}/Arklumos/src/*.cpp"
  "${CMAKE_SOURCE_DIR}/Arklumos/src/*.h"
)

# Add the Arklumos library
# (apparently, after test if STATIC is not used CMake is failing :/)
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
			ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug/${PROJECT_NAME}
			ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/${PROJECT_NAME}
			ARCHIVE_OUTPUT_DIRECTORY_DIST ${CMAKE_SOURCE_DIR}/bin/Dist/${PROJECT_NAME}
			LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug/${PROJECT_NAME}
			LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/${PROJECT_NAME}
			LIBRARY_OUTPUT_DIRECTORY_DIST ${CMAKE_SOURCE_DIR}/bin/Dist/${PROJECT_NAME}
			RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug/${PROJECT_NAME}
			RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/${PROJECT_NAME}
			RUNTIME_OUTPUT_DIRECTORY_DIST ${CMAKE_SOURCE_DIR}/bin/Dist/${PROJECT_NAME}
)

# Set the precompiled header file and include it in the Arklumos
target_precompile_headers(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Arklumos/src/akpch.h)

# Link the Arklumos dll with the glfw, glad, imgui libraries
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	# (as we are using Windows with mingw64, we need also opengl32 gdi32 for glfw)
	target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw opengl32 gdi32 imgui glm stb_image entt yaml-cpp imguizmo)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw imgui glm stb_image entt yaml-cpp imguizmo)# GL dl pthread X11 Xxf86vm Xrandr Xi GLEW)
endif()

# Set the output directory for the Arklumos library
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" OUTPUT_NAME "Arklumos")

# Define macros for the Arklumos library
# (here to specify it's for windows and to build a dll)
# GLFW_INCLUDE_NONE is for glad to work with glfw
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	#Define MACROS
	target_compile_definitions(${PROJECT_NAME} PRIVATE AK_BUILD_DLL AK_DEBUG GLFW_INCLUDE_NONE)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	#Define MACROS
	target_compile_definitions(${PROJECT_NAME} PRIVATE AK_BUILD_DLL AK_DEBUG GLFW_INCLUDE_NONE)
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
message("> END Arklumos library")


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	message("This is a Debug")
#   target_compile_definitions(Arklumos PUBLIC AK_DEBUG)
#   target_compile_options(Arklumos PUBLIC -g)

elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
	message("This is a Release")
#   target_compile_definitions(Arklumos PUBLIC AK_RELEASE)
#   target_compile_options(Arklumos PUBLIC -O2)

elseif (CMAKE_BUILD_TYPE STREQUAL "Dist")
	message("This is a Dist")
#   target_compile_definitions(Arklumos PUBLIC AK_DIST)
#   target_compile_options(Arklumos PUBLIC -O3)

endif()