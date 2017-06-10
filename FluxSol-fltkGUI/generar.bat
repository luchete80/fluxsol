set PATH=D:/Luciano/Programas/x86_64-5.3.0-posix-seh-rt_v4-rev0/mingw64/bin;D:/Luciano/Programas/CMake/bin;%VTK_DIR%/lib;	
set VTK_DIR=D:/Luciano/Libs/VTK-6.3.0-Install/

set CXX=x86_64-w64-mingw32-g++.exe
set CC=x86_64-w64-mingw32-gcc.exe

cmake -G "CodeBlocks - MinGW Makefiles" -DCMAKE_CXX_COMPILER=%CXX% -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=%VTK_DIR%/lib/cmake/vtk-6.3 -DVTK_USE_HYBRID=ON -DVTK_USE_RENDERING=ON D:/Luciano/Numerico/MecSol/FluxSol/Src/FluxSol-GUI

mingw32-make.exe