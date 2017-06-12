set FLUXSOL_DIR=D:/Luciano/Numerico/MecSol/FluxSol/Src
set VTK_DIR=D:/Luciano/Libs/VTK-6.3.0-Install/

set PATH=D:/Luciano/Programas/x86_64-5.3.0-posix-seh-rt_v4-rev0/mingw64/bin;D:/Luciano/Programas/CMake/bin;D:/Luciano/Libs/VTK-6.3.0-Install/bin;D:/Luciano/Programas/msys64/mingw64/lib

set CXX=x86_64-w64-mingw32-g++.exe
set CC=x86_64-w64-mingw32-gcc.exe

@rem set PKG_CONFIG_PATH=D:/Luciano/Programas/msys64/mingw64/lib/pkgconfig

set PKG_CONFIG_PATH=D:/Luciano/Libs/gtk-2.24-bundle/lib/pkgconfig

cmake -G "CodeBlocks - MinGW Makefiles" -DCMAKE_CC_COMPILER=%CC% -DCMAKE_CXX_COMPILER=%CXX% -DCMAKE_BUILD_TYPE=RELEASE %FLUXSOL_DIR%/gtkFluxSol-GUI/tests/gtkglext

mingw32-make.exe