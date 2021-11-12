cmake_minimum_required(VERSION 3.0)
project(test)
set(SOURCES main.cpp)
set(SDL2_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/lib/sdl_dev_lib/include/SDL2")
set(SDL2_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/lib/sdl_dev_lib/lib")  #set custom defined SDL2_LIBRARY_DIR variable to appropriate path
include_directories(${SDL2_INCLUDE_DIR})
add_executable(game WIN32 ${SOURCES})
target_link_libraries(game -lmingw32 ${SDL2_LIBRARY_DIR}/libSDL2main.a ${SDL2_LIBRARY_DIR}/libSDL2.dll.a -mwindows)