set FLUXSOL_DIR=D:/Luciano/Numerico/MecSol/FluxSol/Src
set VTK_DIR=D:/Luciano/Libs/VTK-6.3.0-Install/

set GLFW_DIR=D:/Luciano/Libs/glfw-install
set IMGUI_DIR=D:/Luciano/Libs/imgui-v1.5

set PATH=D:/Luciano/Programas/x86_64-5.3.0-posix-seh-rt_v4-rev0/mingw64/bin;D:/Luciano/Programas/CMake/bin;

set CXX=x86_64-w64-mingw32-g++.exe
set CC=x86_64-w64-mingw32-gcc.exe

cmake -G "CodeBlocks - MinGW Makefiles" -DCMAKE_CXX_COMPILER=%CXX% -DCMAKE_BUILD_TYPE=DEBUG _DCMAKE_CXX_FLAGS="-g" %FLUXSOL_DIR%/FluxSol-GUI/tests/imguivtk/opengl2

mingw32-make.exe