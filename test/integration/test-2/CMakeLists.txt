cmake_minimum_required(VERSION 3.0)
set(CMAKE_C_COMPILER, "gcc-8.0.1")
set(CMAKE_CXX_COMPILER, "g++-8.0.1")

project(test)

set(XIOUDOWN_ENGINE_DIR "${PROJECT_SOURCE_DIR}/lib/Xioudown")
# add_subdirectory(${XIOUDOWN_ENGINE_DIR})

# Do Xioudown engine includes for headers and src code
# set(XIOUDOWN_LIB_DIR "${XIOUDOWN_ENGINE_DIR}/lib")
set(XIOUDOWN_INCLUDE_DIR "${XIOUDOWN_ENGINE_DIR}/include")
set(XIOUDOWN_SOURCE_DIR "${XIOUDOWN_ENGINE_DIR}/src")
set(XIOUDOWN_SOURCES
    ${XIOUDOWN_SOURCE_DIR}/game_io.cpp
    ${XIOUDOWN_SOURCE_DIR}/game_window.cpp
    ${XIOUDOWN_SOURCE_DIR}/utils.cpp
    ${XIOUDOWN_SOURCE_DIR}/polygon.cpp
    ${XIOUDOWN_SOURCE_DIR}/xioudown.cpp
)

# Do SDL includes and stuff
set(SDL2_DEP_DIR "${XIOUDOWN_ENGINE_DIR}/lib/sdl_dev_lib")
set(SDL2_LIBRARY_DIR "${SDL2_DEP_DIR}/lib")  #set custom defined SDL2_LIBRARY_DIR variable to appropriate path
set(SDL2_LIBRARIES ${SDL2_LIBRARY_DIR}/libSDL2main.a ${SDL2_LIBRARY_DIR}/libSDL2_ttf.a ${SDL2_LIBRARY_DIR}/libSDL2_image.a ${SDL2_LIBRARY_DIR}/libSDL2.dll.a)
set(SDL2_INCLUDE_DIR "${SDL2_DEP_DIR}/include/SDL2")

# Do game files to compile
set(GAME_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(GAME_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(GAME_SOURCES
    ${GAME_SOURCE_DIR}/aspects.cpp
)

include_directories(${SDL2_INCLUDE_DIR} ${XIOUDOWN_INCLUDE_DIR} ${GAME_INCLUDE_DIR})

add_executable(${PROJECT_NAME} WIN32 ${XIOUDOWN_SOURCES} ${GAME_SOURCES} main.cpp)
target_link_libraries(${PROJECT_NAME} -lmingw32 ${SDL2_LIBRARIES} -mwindows)
# target_link_libraries(${PROJECT_NAME} -lmingw32 ${SDL2_LIBRARIES} ${XIOUDOWN_LIB_DIR}/Xioudown.lib -mwindows)