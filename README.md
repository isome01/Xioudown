# Xioudown
This is an online backup state for the Xioudown Game Engine

This game-engine makes use of dependencies:
 - CMAKE 3.21.X
 - MinGW 64 compiler
 - SDL_development_lib 32/64 bit
 - SDL_image 32/64 bit
 - SDL_ttf 32/64 bit

MinGW 64 compiler is needed to make applications with the Xioudown Game Engine and it currently
DOES NOT compile with libraries 32-bit or lower. It's therefore recommended to just have the
minimal dependencies that Xioudown compiles with (and any other third-party libraries you may be
using) installed before compiling.

# CMAKE
	Windows: you will need to install CMake for windows. Cmake Generator and build directory must be specified. (Generator can be Visual Studio 16 2019)
		- Add Cmake and MSBuild to PATH once all respective requirements have been installed. (Also be sure to restart terminal and IDE for re-configuration)
		- cmake -S <source_path> -B <build_path> -G "<Generator>" to create a Visual Studio Build solution
		- MSBuild.exe <project_name>.sln

	~ Helpful references:
		- CMake, SDL, and Windows (http://zaunfish.blogspot.com/2020/06/setting-up-sdl2-on-windows-with-mingw.html)
			If Linux/OSX/UNIX: then https://cmake.org/cmake/help/latest/module/FindSDL.html#variables-for-locating-sdl
		- DLLs: https://stackoverflow.com/questions/124549/what-exactly-are-dll-files-and-how-do-they-work
		- Static Librareies (C++ are *.lib and C are *.a extensions) -> https://stackoverflow.com/questions/43772887/cmake-creating-a-static-library

# references
	Casing: https://lefticus.gitbooks.io/cpp-best-practices/content/03-Style.html


link to download mingw64 is https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/

Enjoy :D