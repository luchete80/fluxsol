set PATH=C:/mingw-w64/x86_64-4.9.1-posix-seh-rt_v3-rev0/mingw64/bin;C:/cmake-2.8.12.2-win32-x86/bin
set CC=x86_64-w64-mingw32-gcc.exe
set CXX=x86_64-w64-mingw32-g++.exe

set QT_DIR=D:/Luciano/Numerico/Libs/QtSDK-x86_64-5.2.1-mingw64-gcc-4.8.2-seh
	
set CMAKE_PREFIX_PATH=%QT_DIR%/lib/cmake

cmake -D BUILD_EXAMPLES=0 -D BUILD_SHARED_LIBS=1 -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=%QT_DIR%/lib/cmake -D VTK_Group_Qt=1 -D VTK_QT_VERSION=5 -D PROJ_USE_PTHREADS=0 -D QT_QMAKE_EXECUTABLE=%QT_DIR%/bin -D CMAKE_CXX_FLAGS=-fpermissive -D VTK_Group_Views=1 -DCMAKE_CXX_FLAGS=-fpermissive -DCMAKE_LIBRARY_PATH=C:/Windows/SysWOW64 -G "MinGW Makefiles" ../VTK-6.1.0