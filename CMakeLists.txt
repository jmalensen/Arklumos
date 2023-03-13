# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.24)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "CMake version: ${CMAKE_VERSION}")

# Target Linux
if(UNIX AND NOT APPLE)
	message(STATUS "This is a GNU/Linux system")
  set(CMAKE_SYSTEM_NAME Linux)
	set(CMAKE_SYSTEM_PROCESSOR x86_64)

  set(CMAKE_C_COMPILER    "/usr/bin/gcc")
  set(CMAKE_CXX_COMPILER  "/usr/bin/g++")

  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Target Windows
if(WIN32)
	message(STATUS "This is a Windows system")
  set(CMAKE_SYSTEM_NAME Windows)
endif()


set(PROJECT_NAME Arklumos)
project(ArklumosBase LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# # Set the output directory for the built executable and library
# # because I want a specific output folder
# set(outputdir "${CMAKE_CFG_INTDIR}/${CMAKE_SYSTEM_NAME}-${CMAKE_HOST_SYSTEM_PROCESSOR}")
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputdir}")
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputdir}")
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputdir}")

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

include_directories(${CMAKE_SOURCE_DIR}/Arklumos/vendor/stb_image)
add_subdirectory(${CMAKE_SOURCE_DIR}/Arklumos/vendor/stb_image)

include_directories(${CMAKE_SOURCE_DIR}/Arklumos/vendor/entt/include)
add_subdirectory(${CMAKE_SOURCE_DIR}/Arklumos/vendor/entt)

include_directories(${CMAKE_SOURCE_DIR}/Arklumos/vendor/yaml-cpp/include)
add_subdirectory(${CMAKE_SOURCE_DIR}/Arklumos/vendor/yaml-cpp)

include_directories(${CMAKE_SOURCE_DIR}/Arklumos/vendor/ImGuizmo)
add_subdirectory(${CMAKE_SOURCE_DIR}/Arklumos/vendor/ImGuizmo)

include_directories(${CMAKE_SOURCE_DIR}/Arklumos/src)

# Main targets built by this project
add_subdirectory(Arklumos)
add_subdirectory(Arklusis)
add_subdirectory(Testbox)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "Arklusis")

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