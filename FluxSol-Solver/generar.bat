set FLUXSOL_DIR=D:\Luciano\Numerico\MecSol\FluxSol
set PETSC_DIR=D:\Luciano\Programas\cygwin64\petsc-3.4.4
set MPI_LIB_DIR=D:\Luciano\Programas\Microsoft-HPCPack-2008R2\Lib\amd64
set MPI_INC_DIR=D:\Luciano\Programas\Microsoft-HPCPack-2008R2\Inc
set PETSC_ARCH=arch-mswin-mpi1-release

set PATH=D:\Luciano\Programas\x86_64-4.9.1-posix-seh-rt_v3-rev3\mingw64\bin;D:\Luciano\Programas\cmake-3.6.0-rc4-win64-x64\bin;%PATH%


set PARMETIS_DIR=D:\Luciano\Numerico\Libs\parmetis-4.0.3

set CXX=x86_64-w64-mingw32-g++.exe
set CC=x86_64-w64-mingw32-gcc.exe

cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_CXX_FLAGS="-fpermissive -O3 -ffast-math -m64" -DUSE_PETSC=ON -G "CodeBlocks - MinGW Makefiles" %FLUXSOL_DIR%\Src\FluxSol-Solver

mingw32-make.exe