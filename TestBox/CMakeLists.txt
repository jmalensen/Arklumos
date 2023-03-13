# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.24)

message("> START Testbox")

## TESTBOX PART
# Find the files for the Testbox part
file(GLOB_RECURSE TESTBOX_SRC_FILES
  "${CMAKE_SOURCE_DIR}/Testbox/src/*.cpp"
  "${CMAKE_SOURCE_DIR}/Testbox/src/*.h"
)

# Add the Testbox executable
add_executable(Testbox
		${TESTBOX_SRC_FILES})

set_target_properties(Testbox PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug/Testbox
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/Testbox
		ARCHIVE_OUTPUT_DIRECTORY_DIST ${CMAKE_SOURCE_DIR}/bin/Dist/Testbox
		LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug/Sandbox
		LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/Testbox
		LIBRARY_OUTPUT_DIRECTORY_DIST ${CMAKE_SOURCE_DIR}/bin/Dist/Testbox
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug/Testbox
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/Testbox
		RUNTIME_OUTPUT_DIRECTORY_DIST ${CMAKE_SOURCE_DIR}/bin/Dist/Testbox
)

# Link the Testbox executable with the Arklumos library
target_link_libraries(Testbox PRIVATE ${PROJECT_NAME})# imgui glm entt yaml-cpp)
# target_link_directories(Testbox PRIVATE ${CMAKE_SOURCE_DIR}/Arklumos/vendor/imgui)

# Define macros for the Testbox
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	#Define AK_PLATFORM_WINDOWS
	#target_compile_definitions(Testbox PRIVATE AK_PLATFORM_WINDOWS)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	#Define AK_PLATFORM_LINUX
	#target_compile_definitions(Testbox PRIVATE AK_PLATFORM_LINUX)
endif()

set_property(TARGET Testbox PROPERTY POSITION_INDEPENDENT_CODE ON)
message("> END Testbox")

# set_target_properties(${PROJECT_NAME} Testbox
#     PROPERTIES
#     ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
#     LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
#     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
#     OUTPUT_NAME_DEBUG "Arklumos_d"
#     OUTPUT_NAME_RELEASE "Arklumos"
# )

# if (CMAKE_BUILD_TYPE STREQUAL "Debug")
# 	message("This is a Debug")
#   target_compile_definitions(Testbox PUBLIC AK_DEBUG)
#   target_compile_options(Testbox PUBLIC -g)

# elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
# 	message("This is a Release")
#   target_compile_definitions(Testbox PUBLIC AK_RELEASE)
#   target_compile_options(Testbox PUBLIC -O2)

# elseif (CMAKE_BUILD_TYPE STREQUAL "Dist")
# 	message("This is a Dist")
#   target_compile_definitions(Testbox PUBLIC AK_DIST)
#   target_compile_options(Testbox PUBLIC -O3)

# endif()