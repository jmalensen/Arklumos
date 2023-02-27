
# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.16)

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

include_directories(${CMAKE_SOURCE_DIR}/Arklumos/src)

## ARKLUMOS LIB PART
# Find the files for the Arklumos library
# (should I target each file one by one ?)
file(GLOB_RECURSE SRC_FILES
  "${CMAKE_SOURCE_DIR}/Arklumos/src/*.cpp"
  "${CMAKE_SOURCE_DIR}/Arklumos/src/*.h"
)

# Add the Arklumos library
# (apparently, after test if SHARED is not used CMake is failing :/)
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

# Link the Arklumos dll with the glfw library
# (as we are using Windows with mingw64, we need also opengl32 gdi32 for glfw)
target_link_libraries(${PROJECT_NAME} glad glfw opengl32 gdi32 imgui)

# Set the precompiled header file and include it in the Arklumos
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Arklumos/src/akpch.h)

# Set the output directory for the Arklumos library
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" OUTPUT_NAME "Arklumos")

# Define macros for the Arklumos library
# (here to specify it's for windows and to build a dll)
target_compile_definitions(${PROJECT_NAME} PRIVATE AK_BUILD_DLL AK_PLATFORM_WINDOWS AK_DEBUG GLFW_INCLUDE_NONE)


## TESTBOX PART
# Find the files for the Testbox part
# (should I target each file one by one ?)
# file(GLOB_RECURSE TESTBOX_SRC_FILES
#   "${CMAKE_SOURCE_DIR}/Testbox/src/*.cpp"
#   "${CMAKE_SOURCE_DIR}/Testbox/src/*.h"
# )

# Add the Testbox executable
# (right now, it's easy because there is only file)
add_executable(Testbox Testbox/src/Testbox.cpp)

# Link the Testbox executable with the Arklumos library
target_link_libraries(Testbox PRIVATE ${PROJECT_NAME})

# Define macros for the Testbox
target_compile_definitions(Testbox PRIVATE AK_PLATFORM_WINDOWS)

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