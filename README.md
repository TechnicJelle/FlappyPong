# Flappy Pong
The game is pretty simple: it's just Pong, but the paddles are controlled like Flappy Bird.

The buttons are Left and Right Shift.

Play it right in your browser on PGEtinker!  
https://pgetinker.com/s/9GMVbVTDdxQ  
(Here, the buttons are A and L)

https://github.com/user-attachments/assets/dafd9f25-6ff4-4930-83c8-34ae39071066

## Building
This project uses [Premake](https://premake.github.io) to generate the build files for your specific development environment.

When cloning this project, make sure to clone it recursively, because olcPixelGameEngine is included as submodule.

### Linux (Make)
Assuming you already have a C++ compiler installed. If you don't, your system package manager should have one.  
I use [GCC](https://gcc.gnu.org/), but you can also use [Clang](https://clang.llvm.org/), for example.

1. Install [Premake](https://premake.github.io/download). Your package manager will probably have this, too
2. Run `premake5 gmake2` in the root directory of this project, to generate a makefile
3. Run `make` in the root directory of this project, to build the project

### Windows (Visual Studio)
Assuming you have already have [Visual Studio](https://visualstudio.microsoft.com/) installed.

1. Download [Premake](https://premake.github.io/download) and put it on your path
2. Run `premake5 vs2022` in the root directory of this project
3. Open the generated solution file in Visual Studio
