# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Target Linux
if(UNIX AND NOT APPLE)
	message(STATUS "This is a GNU/Linux system")
  set(CMAKE_SYSTEM_NAME Linux)
	set(CMAKE_SYSTEM_PROCESSOR x86_64)

  set(CMAKE_C_COMPILER    "/usr/bin/gcc")
  set(CMAKE_CXX_COMPILER  "/usr/bin/g++")

  set(CMAKE_POSITION_INDEPENDENT_CODE ON)

	##TODO: To be tested For Linux
	# Set path to librt.a
	#link_directories(/path/to/librt)
	#the second one is supposed to be better
	#target_link_libraries(my_target_name PRIVATE /path/to/librt.a)
	##To be tested For Linux

  # set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  # list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
endif()

# Target Windows
if(WIN32)
	message(STATUS "This is a Windows system")
  set(CMAKE_SYSTEM_NAME Windows)
endif()

set(PROJECT_NAME Arklumos)
project(${PROJECT_NAME} VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Set the output directory for the built executable and library
# because I want a specific output folder
set(outputdir "${CMAKE_CFG_INTDIR}/${CMAKE_SYSTEM_NAME}-${CMAKE_HOST_SYSTEM_PROCESSOR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputdir}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputdir}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputdir}")

# If no CMAKE_BUILD, we set it to Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Add specifics flags depending if the build is for debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG -O3")
endif()

# Add include directories for the Arklumos library
include_directories(${CMAKE_SOURCE_DIR}/Arklumos/vendor/spdlog/include)

# Add Glfw to Arklumos
# When including GLFW as part of your build, you probably don't want to build the GLFW tests, examples and documentation. To disable these, set the corresponding cache variables before adding the GLFW source tree.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(${CMAKE_SOURCE_DIR}/Arklumos/vendor/GLFW/include)
add_subdirectory(${CMAKE_SOURCE_DIR}/Arklumos/vendor/GLFW)

include_directories(${CMAKE_SOURCE_DIR}/Arklumos/vendor/glad/include)
add_subdirectory(${CMAKE_SOURCE_DIR}/Arklumos/vendor/glad)

include_directories(${CMAKE_SOURCE_DIR}/Arklumos/vendor/imgui)
add_subdirectory(${CMAKE_SOURCE_DIR}/Arklumos/vendor/imgui)

include_directories(${CMAKE_SOURCE_DIR}/Arklumos/vendor/glm)
add_subdirectory(${CMAKE_SOURCE_DIR}/Arklumos/vendor/glm)

include_directories(${CMAKE_SOURCE_DIR}/Arklumos/src)

# if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
#   set(GLFW_ROOT_DIR ${CMAKE_SOURCE_DIR}/Arklumos/vendor/GLFW)
#   set(GLFW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Arklumos/vendor/GLFW/include)
#   set(GLFW_LIBRARY ${CMAKE_SOURCE_DIR}/Arklumos/vendor/GLFW)

#   find_package(GLFW REQUIRED)
# endif()

## ARKLUMOS LIB PART
# Find the files for the Arklumos library
# (should I target each file one by one ?)
file(GLOB_RECURSE SRC_FILES
  "${CMAKE_SOURCE_DIR}/Arklumos/src/*.cpp"
  "${CMAKE_SOURCE_DIR}/Arklumos/src/*.h"
)

# Add the Arklumos library
# (apparently, after test if STATIC is not used CMake is failing :/)
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})


# Link the Arklumos dll with the glfw, glad, imgui libraries
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	# (as we are using Windows with mingw64, we need also opengl32 gdi32 for glfw)
	target_link_libraries(${PROJECT_NAME} glad glfw opengl32 gdi32 imgui glm)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_link_libraries(${PROJECT_NAME} glad glfw imgui glm)# GL dl pthread X11 Xxf86vm Xrandr Xi GLEW)
endif()


# Set the precompiled header file and include it in the Arklumos
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Arklumos/src/akpch.h)

# Set the output directory for the Arklumos library
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" OUTPUT_NAME "Arklumos")

# Define macros for the Arklumos library
# (here to specify it's for windows and to build a dll)
# GLFW_INCLUDE_NONE is for glad to work with glfw
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	#Define AK_PLATFORM_WINDOWS
	target_compile_definitions(${PROJECT_NAME} PRIVATE AK_BUILD_DLL AK_PLATFORM_WINDOWS AK_DEBUG GLFW_INCLUDE_NONE)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	#Define AK_PLATFORM_LINUX (check if GLFW_INCLUDE_NONE is still required)
	target_compile_definitions(${PROJECT_NAME} PRIVATE AK_BUILD_DLL AK_PLATFORM_LINUX AK_DEBUG GLFW_INCLUDE_NONE)
endif()


## TESTBOX PART
# Find the files for the Testbox part
# (should I target each file one by one ?)
# file(GLOB_RECURSE TESTBOX_SRC_FILES
#   "${CMAKE_SOURCE_DIR}/Testbox/src/*.cpp"
#   "${CMAKE_SOURCE_DIR}/Testbox/src/*.h"
# )

# Add the Testbox executable
# (right now, it's easy because there is only file)
add_executable(Testbox TestBox/src/Testbox.cpp)

# Link the Testbox executable with the Arklumos library
target_link_libraries(Testbox PRIVATE ${PROJECT_NAME} imgui)
# target_link_directories(Testbox PRIVATE ${CMAKE_SOURCE_DIR}/Arklumos/vendor/imgui)

# Define macros for the Testbox
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	#Define AK_PLATFORM_WINDOWS
	target_compile_definitions(Testbox PRIVATE AK_PLATFORM_WINDOWS)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	#Define AK_PLATFORM_LINUX
	target_compile_definitions(Testbox PRIVATE AK_PLATFORM_LINUX)
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET Testbox PROPERTY POSITION_INDEPENDENT_CODE ON)

set_target_properties(${PROJECT_NAME} Testbox
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    OUTPUT_NAME_DEBUG "Arklumos_d"
    OUTPUT_NAME_RELEASE "Arklumos"
)



if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	message("This is a Debug")
  target_compile_definitions(Arklumos PUBLIC AK_DEBUG)
  target_compile_definitions(Testbox PUBLIC AK_DEBUG)
  target_compile_options(Arklumos PUBLIC -g)
  target_compile_options(Testbox PUBLIC -g)

elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
	message("This is a Release")
  target_compile_definitions(Arklumos PUBLIC AK_RELEASE)
  target_compile_definitions(Testbox PUBLIC AK_RELEASE)
  target_compile_options(Arklumos PUBLIC -O2)
  target_compile_options(Testbox PUBLIC -O2)

elseif (CMAKE_BUILD_TYPE STREQUAL "Dist")
	message("This is a Dist")
  target_compile_definitions(Arklumos PUBLIC AK_DIST)
  target_compile_definitions(Testbox PUBLIC AK_DIST)
  target_compile_options(Arklumos PUBLIC -O3)
  target_compile_options(Testbox PUBLIC -O3)

endif()



# Still testing
# if(CMAKE_BUILD_TYPE STREQUAL "Dist")
  # add_library(Arklumos_static STATIC ${SRC_FILES})
  # set_target_properties(Arklumos_static PROPERTIES PREFIX "" OUTPUT_NAME "Arklumos")
  # target_compile_definitions(Arklumos_static PRIVATE AK_PLATFORM_WINDOWS)

  # set_target_properties(Arklumos_static
  #   PROPERTIES
  #   ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
  #   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
  #   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
  #   OUTPUT_NAME "Arklumos"
  # )

  # target_compile_definitions(Arklumos_static PUBLIC AK_DIST)
# endif()