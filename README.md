# Flappy Pong

## Building
This project uses premake to generate the build files for your specific development environment.

When cloning this project, make sure to clone it recursively! The PGE is included as submodule.

### Linux
1. Install a C++ compiler. Your package manager should have one. I use [GCC](https://gcc.gnu.org/), but you can also use [Clang](https://clang.llvm.org/), for example.
2. Install [Premake](https://premake.github.io/download). Your package manager will probably have this, too
3. Run `premake5 gmake2` in the root directory of this project, to generate a makefile
4. Run `make` in the root directory of this project, to build the project

### Windows (Visual Studio)
(Assuming you have [Visual Studio](https://visualstudio.microsoft.com/) already installed)
1. Download [Premake](https://premake.github.io/download) and put it on your path
2. Run `premake5 vs2022` in the root directory of this project
3. Open the generated solution file in Visual Studio
