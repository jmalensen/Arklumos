
cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME Arklumos)
set(ARKLUMOSFINAL_SOURCE_DIR src)
#set(ARKLUMOSFINAL_SOURCE ${TEMPLATEC_SOURCE_DIR}/main.cpp)

project(${PROJECT_NAME} VERSION 0.1.0)

include(CTest)
enable_testing()

# I../includes
# include_directories(vendor/spdlog/include)
# link_directories(vendor/spdlog/include)

add_definitions(-DAK_BUILD_DLL)

# Set source files
set(SOURCES
    src/Arklumos/Application.cpp
    src/Arklumos/EntryPoint.h
)

# It's going to be a library
add_library(${PROJECT_NAME} SHARED ${SOURCES})



# Define the entry point header file for the library
# set(ENTRY_POINT src/Arklumos/EntryPoint.h)

# Add the required headers to the library target
#target_sources(${PROJECT_NAME} INTERFACE ${HEADERS})



set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# Set library properties
set_target_properties(${PROJECT_NAME} PROPERTIES
		CMAKE_CXX_STANDARD 20
    CMAKE_CXX_STANDARD_REQUIRED ON
    PUBLIC_HEADER src/Arklumos.h
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
