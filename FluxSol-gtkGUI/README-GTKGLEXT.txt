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
3.1) Download libtool from msys setup (con el pacman), junto con automake, autoconf, make

3.2 Correr: 
autoreconf -i

3.3) en el archvo configure, Commentar line 16781
##################################################
# GObject introspection
##################################################

ESTA----> #GOBJECT_INTROSPECTION_CHECK(1.30.0)

Comentar tambien la linea of check libx11

3.4 COMENTAR LA OPCION DE x11 en el configure

3.5 

export PKG_CONFIG_PATH="/d/Luciano/Programas/msys64/mingw64/lib/pkgconfig"
export PATH=/d/Luciano/Programas/x86_64-5.3.0-posix-seh-rt_v4-rev0/mingw64/bin:$PATH
export PATH=/d/Luciano/Programas/msys64/mingw64/bin:$PATH


3.6
env CC='x86_64-w64-mingw32-gcc' AR=/d/Luciano/Programas/x86_64-5.3.0-posix-seh-rt_v4-rev0/mingw64/bin/ar.exe ./configure --prefix=/cygdrive/d/Luciano/Libs/gtkglext-1.2.0 --build=i386-pc-mingw32 --disable-static --disable-debug --enable-win32-backend --disable-x11-backend


--------------------------
LUEGO SE COMENTA LA LINEA: 
//#include <gdk/gdkglcontext.h>
EN EL INCLUDE DE GTK (mingw64/include/gtk-3/gdk/gdk.h)
ESTO ES PORQUE 
