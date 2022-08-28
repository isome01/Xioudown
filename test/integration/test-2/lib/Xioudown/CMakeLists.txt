cmake_minimum_required(VERSION 3.21.0)

set(CMAKE_C_COMPILER, "gcc-8.0.1")
set(CMAKE_CXX_COMPILER, "g++-8.0.1")

# set the project name and version
project(Xioudown VERSION 1.0 DESCRIPTION "Xioudown Game Engine build testing")
# Set source directories
set(XIOUDOWN_SOURCES src/io.cpp src/window.cpp src/methods.cpp src/polygon.cpp)

# first find and intiate SDL: Custom
message(STATUS "Linking SDL from source: ${PROJECT_SOURCE_DIR}/lib/sdl_dev_lib")
set(SDL2_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/lib/sdl_dev_lib/include/SDL2")

# Include SDL
include_directories(${SDL2_INCLUDE_DIR})

set(SDL2_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/lib/sdl_dev_lib/lib")  #set custom defined SDL2_LIBRARY_DIR variable to appropriate path
# Here we'll have to explicitly preset the SDL Dependencies/Libraries
set(SDL2_LBRARIES ${SDL2_LIBRARY_DIR}/libSDL2main.a ${SDL2_LIBRARY_DIR}/libSDL2_ttf.a ${SDL2_LIBRARY_DIR}/libSDL2_image.a ${SDL2_LIBRARY_DIR}/libSDL2.dll.a)

# Create Library from sources
add_library("${PROJECT_NAME}" STATIC ${XIOUDOWN_SOURCES})
# add_executable("${PROJECT_NAME}" WIN32 ${XIOUDOWN_SOURCES})
target_include_directories(Xioudown PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Xioudown -lmingw32 ${SDL2_LBRARIES} -mwindows)