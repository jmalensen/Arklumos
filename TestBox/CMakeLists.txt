
cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME Testbox)
set(ARKLUMOSFINAL_SOURCE_DIR src)
# set(ARKLUMOSFINAL_SOURCE ${TEMPLATEC_SOURCE_DIR}/Testbox.cpp)

project(${PROJECT_NAME} VERSION 0.1.0)

include(CTest)
enable_testing()

# Add subdirectory for external library (the game engine)
set(Arklumos_BUILD_DIR ${CMAKE_BINARY_DIR}/Arklumos)
add_subdirectory(../Arklumos ${Arklumos_BUILD_DIR})

# Set include directories
include_directories(
	# ../vendor/spdlog/include
	../Arklumos
)
message(${CMAKE_BINARY_DIR})

# link_directories("../vendor/spdlog/include")
link_directories("../Arklumos")

# puts all .cpp files inside src to the SRC_FILES variable
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# compiles the files defined by SRC_FILES to generante the executable defined by EXEC
add_executable(${PROJECT_NAME} ${SRC_FILES})

# target_link_libraries( ${PROJECT_NAME} spdlog )
target_link_libraries( ${PROJECT_NAME} Arklumos )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# set the C++20 standard
set_target_properties(${PROJECT_NAME} PROPERTIES
		CMAKE_CXX_STANDARD 20
    CMAKE_CXX_STANDARD_REQUIRED ON
)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# UNIX, WIN32, WINRT, CYGWIN, APPLE are environment variables as flags set by default system
if(UNIX)
    message("This is a ${CMAKE_SYSTEM_NAME} system")
elseif(WIN32)
    message("This is a Windows System")
endif()# or use MATCHES to see if actual system name 

# Darwin is Apple's system name
if(${CMAKE_SYSTEM_NAME} MATCHES Darwin)
    message("This is a ${CMAKE_SYSTEM_NAME} system")
elseif(${CMAKE_SYSTEM_NAME} MATCHES Windows)
    message("This is a Windows System")
endif()

include(CPack)
