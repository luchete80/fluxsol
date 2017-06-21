set FLUXSOL_DIR=D:/Luciano/Numerico/MecSol/FluxSol/Src
set VTK_DIR=D:/Luciano/Libs/VTK-7.1.1-Install/

set PATH=D:/Luciano/Programas/x86_64-5.3.0-posix-seh-rt_v4-rev0/mingw64/bin;D:/Luciano/Programas/CMake/bin;

set GODOTGUI_BIN_DIR=D:/Luciano/Libs/GodotGUI-Bin-Debug
set FLUXSOL_DIR=D:\Luciano\Numerico\MecSol\FluxSol
set PETSC_DIR=D:\Luciano\Programas\cygwin64\petsc-3.4.4
set MPI_LIB_DIR=D:\Luciano\Programas\Microsoft-HPCPack-2008R2\Lib\amd64
set MPI_INC_DIR=D:\Luciano\Programas\Microsoft-HPCPack-2008R2\Inc
set PETSC_ARCH=arch-mswin-mpi1-release

set CXX=x86_64-w64-mingw32-g++.exe
set CC=x86_64-w64-mingw32-gcc.exe

cmake -G "CodeBlocks - MinGW Makefiles" -DCMAKE_CXX_COMPILER=%CXX% -DCMAKE_BUILD_TYPE=DEBUG _DCMAKE_CXX_FLAGS="-g" %FLUXSOL_DIR%/FluxSol-GUI

mingw32-make.exe