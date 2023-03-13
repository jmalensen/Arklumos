# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.24)

## ARKLUSIS PART
# Find the files for the Testbox part
file(GLOB_RECURSE ARKLUSIS_SRC_FILES
  "${CMAKE_SOURCE_DIR}/Arklusis/src/*.cpp"
  "${CMAKE_SOURCE_DIR}/Arklusis/src/*.h"
)

# Add the Testbox executable
add_executable(Arklusis
		${ARKLUSIS_SRC_FILES})

# Link the Arklusis executable with the Arklumos library
target_link_libraries(Arklusis PRIVATE ${PROJECT_NAME} glm entt imguizmo)

set_property(TARGET Arklusis PROPERTY POSITION_INDEPENDENT_CODE ON)