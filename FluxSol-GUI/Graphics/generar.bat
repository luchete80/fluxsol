set FLUXSOL_DIR=D:/Luciano/Numerico/MecSol/FluxSol
set VTK_DIR=D:/Luciano/Libs/VTK-6.3.0-Install/

set PATH=D:/Luciano/Programas/x86_64-5.3.0-posix-seh-rt_v4-rev0/mingw64/bin;D:/Luciano/Programas/CMake/bin;D:/Luciano/Libs/VTK-6.3.0-Install/bin;D:/Luciano/Programas/msys64/mingw64/lib

set GODOTGUI_BIN_DIR=D:/Luciano/Libs/GodotGUI-Bin-Debug
set PETSC_DIR=D:/Luciano/Programas/cygwin64/petsc-3.4.4
set MPI_LIB_DIR=D:/Luciano/Programas/Microsoft-HPCPack-2008R2/Lib/amd64
set MPI_INC_DIR=D:/Luciano/Programas/Microsoft-HPCPack-2008R2/Inc
set PETSC_ARCH=arch-mswin-mpi1-release

set CXX=x86_64-w64-mingw32-g++.exe
set CC=x86_64-w64-mingw32-gcc.exe

@rem set PKG_CONFIG_PATH=D:/Luciano/Programas/msys64/mingw64/lib/pkgconfig

cmake -G "CodeBlocks - MinGW Makefiles" -DCMAKE_CC_COMPILER=%CC% -DCMAKE_CXX_COMPILER=%CXX% -DUSE_PETSC=ON -DCMAKE_CXX_FLAGS="-g" -DCMAKE_BUILD_TYPE=DEBUG %FLUXSOL_DIR%/Src/FluxSol-GUI/

mingw32-make.exe