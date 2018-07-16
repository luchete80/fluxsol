set PATH=D:\Luciano\Programas\mingw64\bin;D:\Luciano\Programas\cmake\bin

set VTK_DIR=D:\Luciano\Numerico\Libs\vtk-6.3.0-release-qt-opengl-sin-threads

set QT_DIR=D:\Luciano\Numerico\Libs\QtSDK-5.2.1-x86_64-seh

set FLUXSOL_DIR=D:\Luciano\Numerico\MecSol\FluxSol
set FLUXSOL_EXTLIBS_DIR=D:\Luciano\Numerico\Libs

set PETSC_DIR=D:\Luciano\Programas\cygwin64\petsc-3.4.4
set PETSC_ARCH=arch-mswin-mpi1-release

set MPI_LIB_DIR=D:\Luciano\Programas\Microsoft-HPCPack-2008R2\Lib\amd64
set MPI_INC_DIR=D:\Luciano\Programas\Microsoft-HPCPack-2008R2\Inc

set PARMETIS_DIR=D:\Luciano\Numerico\Libs\parmetis-4.0.3

set PATH=D:\Luciano\Programas\mingw\bin;D:\Luciano\Programas\cmake\bin;%FLUXSOL_DIR%\Bin_CodeBlocks\FluxSol;%VTK_DIR%

set CMAKE_PREFIX_PATH=%QT_DIR%;C:\Windows\SysWOW64;%QT_DIR%\lib\cmake

cmake CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=%QT_DIR%\lib\cmake -D VTK_QT_VERSION=5 -D QT_QMAKE_EXECUTABLE=%QT_DIR%\bin -D CMAKE_CXX_FLAGS=-fpermissive -D CMAKE_LIBRARY_PATH=C:\Windows\SysWOW64 -G "CodeBlocks - MinGW Makefiles" ..\Src

mingw32-make.exe