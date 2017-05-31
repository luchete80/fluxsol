/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: gVTKRenderWindowInteractor.h,v $
  Language:  C++
  Date:      $Date: 2009/03/03 16:20:43 $
  Version:   $Revision: 1.24 $

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// .NAME  gVTKRenderWindowInteractor - class to enable VTK to render to
// and interact with wxWindow.
// .SECTION Description
//  gVTKRenderWindowInteractor provide a VTK widget for wxWindow. This class
// was completely rewrote to have the 'Look & Feel' of the python version:
// gVTKRenderWindowInteractor.py
// .SECTION Caveats
//  - There is a know bug that prevent this class to works for more info see
// WX_USE_X_CAPTURE. This bug only affect wxGTK from 2.3.2 to wxGTK 2.4.0.
//  - Furthermore this class is tempated over either wxWindows or wxGLCanvas,
// in wxWindows 2.3.1 and earlier, the wxGLCanvas had scroll bars, you can avoid
// this effect by playing with WX_BASE_CLASS at your risk (you might end up with
// lot of flickering.)
//  - This class might not be easily readable as it tried to work with VTK 3.2
//  and 4.x. This class doesn't support reparenting with VTK 4.2 and earlier.
// .SECTION see also
// gVTKRenderWindowInteractor.py wxVTKRenderWindow.py

#ifndef _gVTKRenderWindowInteractor_h_
#define _gVTKRenderWindowInteractor_h_

// vtk includes
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"

//Godot
#include "canvas_item.h"

#include "windows.h"
#include "./scene/main/timer.h"
#include "./core/os/input_event.h"
#include "scene/gui/control.h"

// wx forward declarations
// class wxPaintEvent;
// class wxMouseEvent;
// class wxTimerEvent;
// class wxKeyEvent;
// class wxSizeEvent;

// #if defined(__WXGTK__) && defined(USE_WXGLCANVAS)
// class godVTKRenderWindowInteractor : public wxGLCanvas, public vtkRenderWindowInteractor
// #else
// class godVTKRenderWindowInteractor : public wxWindow, public vtkRenderWindowInteractor
// #endif //__WXGTK__
class gVTKRenderWindowInteractor :
public Control,
public vtkRenderWindowInteractor
{

    //OBJ_TYPE(GraphEdit,Control);
  //DECLARE_DYNAMIC_CLASS(gVTKRenderWindowInteractor)

  public:
    //constructors
    gVTKRenderWindowInteractor();

    gVTKRenderWindowInteractor(//wxWindow *parent,
                                HWND *parent,
                                int id,//wxWindowID id,
                                const POINT &pos,//const wxPoint &pos = wxDefaultPosition,
                                const Vector2 &size, //const wxSize &size = wxDefaultSize,
                                long style, //= wxWANTS_CHARS | wxNO_FULL_REPAINT_ON_RESIZE,
                                //const wxString &name = wxPanelNameStr);
                                const String &name = "Test");
    vtkTypeRevisionMacro(gVTKRenderWindowInteractor,vtkRenderWindowInteractor);
    static gVTKRenderWindowInteractor * New();
    void PrintSelf(ostream& os, vtkIndent indent);

	  //destructor
    ~gVTKRenderWindowInteractor();

    // vtkRenderWindowInteractor overrides
    void Initialize();
    void Enable();
    bool Enable(bool enable);
    void Disable();
    void Start();
    void UpdateSize(int x, int y);
    int CreateTimer(int timertype);
    int DestroyTimer();
    void TerminateApp() {};

    // event handlers
//     void OnPaint(wxPaintEvent &event);
//     void OnEraseBackground (wxEraseEvent& event);
//     void OnMotion(wxMouseEvent &event);
//
//     void OnButtonDown(wxMouseEvent &event);
//     void OnButtonUp(wxMouseEvent &event);
// #if !(VTK_MAJOR_VERSION == 3 && VTK_MINOR_VERSION == 1)
//     void OnEnter(wxMouseEvent &event);
//     void OnLeave(wxMouseEvent &event);
//     void OnMouseWheel(wxMouseEvent& event);


     void OnPaint();
     void OnEraseBackground ();
     //MOUSE COULD BE THE STRUCT InputEventMouse
     void OnMotion(InputEventMouseMotion &event);
     void OnButtonDown(InputEventMouseButton &event);
     void OnButtonUp(InputEventMouseButton &event);
 #if !(VTK_MAJOR_VERSION == 3 && VTK_MINOR_VERSION == 1)
     //void OnEnter(wxMouseEvent &event);
     //void OnLeave(wxMouseEvent &event);
     void OnMouseWheel(InputEventMouseButton& event);

 //#if wxCHECK_VERSION(2, 8, 0)
//     void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);
 //#endif
     void OnKeyDown(InputEventKey &event);
     void OnKeyUp(InputEventKey &event);
     void OnChar(InputEventKey &event);
 #endif
     void OnTimer();//void OnTimer(wxTimerEvent &event);
     void OnSize(); //void OnSize(wxSizeEvent &event);

    void Render();
    void SetRenderWhenDisabled(int newValue);

    // Description:
    // Prescribe that the window be created in a stereo-capable mode. This
    // method must be called before the window is realized. Default if off.
    vtkGetMacro(Stereo,int);
    vtkBooleanMacro(Stereo,int);
    virtual void SetStereo(int capable);

    // Description:
    // As CaptureMouse could be a problem sometimes on a window box
    // This method allow to set or not the CaptureMouse.
    // This method actually will works only if WX_USE_X_CAPTURE was set to 1
    vtkSetMacro(UseCaptureMouse,int);
    vtkBooleanMacro(UseCaptureMouse,int);

#if VTK_MAJOR_VERSION > 5 || (VTK_MAJOR_VERSION == 5 && VTK_MINOR_VERSION >= 2)
  protected:
    virtual int InternalCreateTimer(int timerId, int timerType, unsigned long duration);
    virtual int InternalDestroyTimer(int platformTimerId);
#endif

  protected:
    //wxTimer timer;
    Timer *timer; //Godot
    int ActiveButton;
    long GetHandleHack();
    int Stereo;

  private:
    long Handle;
    bool Created;
    int RenderWhenDisabled;
    int UseCaptureMouse;

    //DECLARE_EVENT_TABLE()
};

#endif //_godVTKRenderWindowInteractor_h_
