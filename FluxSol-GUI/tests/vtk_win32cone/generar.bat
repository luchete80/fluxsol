rem este es el directorio donde esta instalado
set VTK_DIR=D:/Luciano/Libs/VTK-6.3.0-Install/
set PATH=D:/Luciano/Programas/cmake-3.6.0-rc4-win64-x64/bin;D:/Luciano/Programas/x86_64-4.9.1-posix-seh-rt_v3-rev3/mingw64/bin;%VTK_DIR%/lib;	
rem D:/Luciano/Libs/VTK-6.3.0-Install/include/vtk-6.3;D:/Luciano/Libs/VTK-6.3.0-Install/lib

set CC="x86_64-w64-mingw32-gcc.exe"
set CXX="x86_64-w64-mingw32-g++.exe"

cmake -G "CodeBlocks - MinGW Makefiles" -DCMAKE_C_COMPILER=%CC% -DCMAKE_CXX_COMPILER=%CC% -D CMAKE_INSTALL_PREFIX=%VTK_DIR%/lib/cmake/vtk-6.3 -DVTK_USE_HYBRID=ON -DVTK_USE_RENDERING=ON ../../GodotGUI/tests/win32cone