///////////////////////////////////////////////////////////////


FluxSol is generated with the Following Libraries:



//////////////////////////////////////////////////////////////
 BUILDING PROCESS

1 - At first mut be generated CodeWriter project.
2 - Run CodeWriter Will Generate UDO Creator.
3 - These two steps can be overriden by copying the original UDOCreator



///////////////////// generate.bat ///////////////////////////

set LIBS_DIR=D:/Luciano/Numerico/Libs

set PATH=D:/Programas/x86_64-4.9.1-posix-seh-rt_v3-rev0/mingw64/bin;D:/Luciano/Programas/cmake-2.8.12.2-win32-x86/bin

set VTK_DIR=%LIBS_DIR%/vtk-6.3.0-release-qt-opengl-sin-threads

set QT_DIR=D:/Luciano/Numerico/Libs/QtSDK-5.2.1-x86_64-seh

set FLUXSOL_DIR=D:/Numerico/FluxSol

set PETSC_DIR=D:/Programas/cygwin64/petsc-3.4.4
set PETSC_ARCH=arch-mswin-mpi1-release

set MPI_LIB_DIR=D:/Programas/Microsoft-HPCPack-2008R2/Lib/amd64
set MPI_INC_DIR=D:/Programas/Microsoft-HPCPack-2008R2/Inc

set PARMETIS_DIR=D:/Luciano/Numerico/Libs/parmetis-4.0.3

set PATH=D:/Programas/x86_64-4.9.1-posix-seh-rt_v3-rev0/mingw64/bin;D:/Programas/cmake-2.8.12.2-win32-x86/bin;%FLUXSOL_DIR%/Bin_CodeBlocks/FluxSol;%VTK_DIR%

set CMAKE_PREFIX_PATH=%QT_DIR%;C:/Windows/SysWOW64;%QT_DIR%/lib/cmake

cmake CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=%QT_DIR%/lib/cmake -D VTK_QT_VERSION=5 -D QT_QMAKE_EXECUTABLE=%QT_DIR%/bin -D CMAKE_CXX_FLAGS=-fpermissive -D CMAKE_LIBRARY_PATH=C:/Windows/SysWOW64 -G "CodeBlocks - MinGW Makefiles" ../Src

mingw32-make.exe

///////////////////////////////////////////////////////////////
BUGS AND CRASHES
///////////////////////////////////////////////////////////////
1. For the new GUI libstdc++-6.dll must be put in the folder. But still crash when run from DOS
2. when vtk is linked, crahses.

