#OBJS specifies which files to compile as part of the project
OBJS =  src\aspects.cpp src\client.cpp src\entity.cpp
XIOUDOWN_OBJS = lib\Xioudown\src\grid.cpp lib\Xioudown\src\io.cpp lib\Xioudown\src\polygon.cpp lib\Xioudown\src\utils.cpp lib\Xioudown\src\window.cpp

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
XIOUDOWN_INCLUDE_PATHS = -Ilib\Xioudown\include

SDL2_INCLUDE_PATHS = -Ilib\Xioudown\lib\sdl_dev_lib\include\SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -Llib\Xioudown\lib\sdl_dev_lib\lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w -Wl,-subsystem,windows
COMPILER_FLAGS_DEBUG = -w
#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = test

# test: toy.cpp
# 	$(CC) toy.cpp $(SDL2_INCLUDE_PATHS) -Itest-include $(LIBRARY_PATHS) -w -Wl,-subsystem,windows $(LINKER_FLAGS) -o build\Debug\$(OBJ_NAME)

debug : main.cpp $(XIOUDOWN_OBJS) $(OBJS)
	$(CC) $(XIOUDOWN_OBJS) $(OBJS) main.cpp $(SDL2_INCLUDE_PATHS) $(XIOUDOWN_INCLUDE_PATHS) -Iinclude -Ilib\Xioudown\include $(LIBRARY_PATHS) $(COMPILER_FLAGS_DEBUG) $(LINKER_FLAGS) -o build\Debug\$(OBJ_NAME)

#This is the target that compiles our executable
all : main.cpp $(XIOUDOWN_OBJS) $(OBJS)
	$(CC) $(XIOUDOWN_OBJS) $(OBJS) main.cpp $(SDL2_INCLUDE_PATHS) $(XIOUDOWN_INCLUDE_PATHS) -Iinclude -Ilib\Xioudown\include $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o build\Debug\$(OBJ_NAME)