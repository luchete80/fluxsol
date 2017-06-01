set FLUXSOL_DIR=D:/Luciano/Numerico/MecSol/FluxSol/Src
set VTK_DIR=D:/Luciano/Libs/VTK-6.3.0-Install/

set PATH=D:/Luciano/Programas/x86_64-5.3.0-posix-seh-rt_v4-rev0/mingw64/bin;D:/Luciano/Programas/CMake/bin;

set GODOTGUI_BIN_DIR=D:/Luciano/Libs/GodotGUI-Bin
set CXX=x86_64-w64-mingw32-g++.exe
set CC=x86_64-w64-mingw32-gcc.exe

cmake -G "CodeBlocks - MinGW Makefiles" -DCMAKE_CXX_COMPILER=%CXX% -DCMAKE_BUILD_TYPE=DEBUG _DCMAKE_CXX_FLAGS="-g" %FLUXSOL_DIR%/FluxSol-GUI

mingw32-make.exe