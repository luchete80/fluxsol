MADE BY LUCIANO

1) ATTENTION, MINGW Bin directory must not be in path, because is crashing with 0XC00007b
2) In order to configure vtk : 
There are 3 possible options:
2.1 gtkvtk
2.2 visu (visualization program)
2.3 vtkmm

The last 2 do not use vtkrenderinteractor, whereas the first and third requires gtkglext, which is not ready for gtk3. 
For that, it is convenient to download this port to gtk3:
https://github.com/tdz/gtkglext


3) 

Run autoreconf -i 


////////////////////////
For configure gtkglext
///////////////////////
IN CYGWIN: 
----------
export PKG_CONFIG_PATH=/cygdrive/d/Luciano/Programas/msys64/mingw64/lib/pkgconfig
export PATH=/cygdrive/d/Luciano/Programas/x86_64-5.3.0-posix-seh-rt_v4-rev0/mingw64/bin:$PATH
export PATH=/cygdrive/d/Luciano/Programas/msys64/mingw64/bin:$PATH

NOT THIS : ln -s /cygdrive/d/Luciano/Programas/msys64/mingw64 /target

cd /cygdrive/d/Luciano/Libs/gtkglext3-master

 env CC='x86_64-w64-mingw32-gcc' AR=/usr/bin/ar ./configure --prefix=/cygdrive/d/Luciano/Libs/gtkglext-1.2.0 --build=i386-pc-mingw32 --disable-static 
					
					
					
///////////////////// CON MSYS //////////////////////////
export PKG_CONFIG_PATH=/d/Luciano/Programas/msys64/mingw64/lib/pkgconfig
export PATH=/d/Luciano/Programas/x86_64-5.3.0-posix-seh-rt_v4-rev0/mingw64/bin:$PATH
export PATH=/d/Luciano/Programas/msys64/mingw64/bin:$PATH

 env CC='x86_64-w64-mingw32-gcc -march=pentium' \
        AR=/usr/bin/ar \
        ./configure --prefix=/d/Luciano/Libs/gtkglext-1.2.0 \
                    --build=i386-pc-mingw32 \
                    --disable-static \
                    --enable-debug=no
