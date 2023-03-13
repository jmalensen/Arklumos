# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.24)

message("> START Arklusis")

## ARKLUSIS PART
# Find the files for the Testbox part
file(GLOB_RECURSE ARKLUSIS_SRC_FILES
  "${CMAKE_SOURCE_DIR}/Arklusis/src/*.cpp"
  "${CMAKE_SOURCE_DIR}/Arklusis/src/*.h"
)

# Add the Testbox executable
add_executable(Arklusis
		${ARKLUSIS_SRC_FILES})

set_target_properties(Arklusis PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug/Arklusis
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/Arklusis
		ARCHIVE_OUTPUT_DIRECTORY_DIST ${CMAKE_SOURCE_DIR}/bin/Dist/Arklusis
		LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug/Arklusis
		LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/Arklusis
		LIBRARY_OUTPUT_DIRECTORY_DIST ${CMAKE_SOURCE_DIR}/bin/Dist/Arklusis
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug/Arklusis
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/Arklusis
		RUNTIME_OUTPUT_DIRECTORY_DIST ${CMAKE_SOURCE_DIR}/bin/Dist/Arklusis
)

add_dependencies(Arklusis
		${PROJECT_NAME}
)

# Link the Arklusis executable with the Arklumos library
target_link_libraries(Arklusis PRIVATE ${PROJECT_NAME})# glm entt imguizmo)

set_property(TARGET Arklusis PROPERTY POSITION_INDEPENDENT_CODE ON)
message("> END Arklusis")