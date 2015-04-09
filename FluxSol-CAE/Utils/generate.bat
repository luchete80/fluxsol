set PATH=D:\Luciano\Programas\x86_64-4.9.1-posix-seh-rt_v3-rev0\mingw64\bin;D:\Luciano\Programas\cmake-2.8.12.2-win32-x86\bin

set VTK_DIR=D:\Luciano\Numerico\Libs\VTK-6.1.0-bin-opengl-sin-threads

set QT_DIR=D:\Luciano\Numerico\Libs\QtSDK-5.2.1-x86_64-seh

set FLUXSOL_DIR=D:\Luciano\Numerico\MecSol\FluxSol

set CMAKE_PREFIX_PATH=%QT_DIR%;C:\Windows\SysWOW64;%QT_DIR%\lib\cmake

cmake CMAKE_BUILD_TYPE=Debug -D CMAKE_INSTALL_PREFIX=%QT_DIR%\lib\cmake -D VTK_QT_VERSION=5 -D QT_QMAKE_EXECUTABLE=%QT_DIR%\bin -D CMAKE_CXX_FLAGS=-fpermissive -D CMAKE_LIBRARY_PATH=C:\Windows\SysWOW64 -G "CodeBlocks - MinGW Makefiles" %FLUXSOL_DIR%\Src\FluxSol-CAE

mingw32-make.exe