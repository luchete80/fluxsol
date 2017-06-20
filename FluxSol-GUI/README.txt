http://www.vtk.org/gitweb?p=VTK.git;a=commit;h=2eb0792e

// THIS COMMIT INITIALIZES FROM A CURRENT OPENGL WINDOWS CONTEXT

vtkRenderWindow IntializeFromCurrentContext

/////////////////////////////////////////////////////////////
This is the test
/////////////////////////////////////////////////////////////
///////
//// http://public.kitware.com/pipermail/vtk-developers/2012-November/028013.html

[vtk-developers] Link an existing OpenGL context to a	vtkRenderWindow instance

Joachim Pouderoux joachim.pouderoux at kitware.com 
Wed Nov 21 05:38:58 EST 2012
Previous message: [vtk-developers] Shared ownership of an array of pointers
Next message: [vtk-developers] [VTK 0013676]: vtkLightActor crashes if the light	is not positional
Messages sorted by: [ date ] [ thread ] [ subject ] [ author ]
Hi,

The following commit:
http://vtk.org/gitweb?p=VTK.git;a=commit;h=2eb0792e
introduces a new method IntializeFromCurrentContext() method in
vtkRenderWindow an derived classes (vtk[Win32|X|Cocoa]OpenGLRenderWindow).

This method has been developed to link an preexisting rendering
context/render window to an instance of vtkRenderWindow (an OpenGL context
of a vtkOpenGLRenderWindow to be more precise).
The actual behaviour of vtkRenderWindow performed just before the first
rendering step is:
 - create a new rendering window and link it to a new OpenGL context if
some native window ids/handles (for instance X Display+Drawable on Linux,
HWND and HDC on Windows...) was not previously set by the user
 - OR create and link a new OpenGL context to the window the user defined
by furnishing the previously cited window handles.

This behavior is enough in most situations but in some case you may want to
associate the vtkRenderWindow to a window created by a GUI API which do not
give you access to the native window handles (Java SWT for instance).
However, this API may offer you a way to create an OpenGL canvas in which
you would like to make the VTK visualization.
This is where the new IntializeFromCurrentContext() method is useful: from
the current OpenGL context it retrieves the associated window handles (low
level OpenGL context API offer such functionalities) and associate it to
the vtkRenderWindow instance together with the OpenGL context. In this
particular case VTK will not create window nor OpenGL context any more but
will inherit it from the one discovered by the new function.

You will find here:
http://www.vtk.org/Wiki/VTK/Examples/Cxx/GLUT
an example of how to make it work with the famous GLUT library.

What do you think about that? Any suggestions?

PS : the code has been tested under Windows & Linux - however the Cocoa
(MacOS X) code is available too.
PS2 : for now, vtk*OpenGLRenderWindow destroy the OpenGL context when they
are deleted which may not always be a good idea. I will had a member
variable to avoid that when the class has not created the context by itself.
