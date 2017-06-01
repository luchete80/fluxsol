/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: gVTKRenderWindowInteractor.cxx,v $
  Language:  C++
  Date:      $Date: 2011/12/02 17:46:56 $
  Version:   $Revision: 1.51 $

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <assert.h>

#include "gVTKRenderWindowInteractor.h"

//This is needed for vtk 3.1 :
#ifndef VTK_MAJOR_VERSION
#  include "vtkVersion.h"
#endif

#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
#  include "vtkCommand.h"
#else
#  include "vtkInteractorStyle.h"
#endif
#include "vtkDebugLeaks.h"

#define ID_gVTKRenderWindowInteractor_TIMER 1001

#include "./scene/gui/control.h"

//Keep this for compatibilty reason, this was introduced in wxGTK 2.4.0
// #if (!wxCHECK_VERSION(2, 4, 0))
// wxWindow* wxGetTopLevelParent(wxWindow *win)
// {
    // while ( win && !win->IsTopLevel() )
         // win = win->GetParent();
    // return win;
// }
// #endif


//THIS IS REMOVED IN VERSION 6, LUCIANO
//vtkCxxRevisionMacro(gVTKRenderWindowInteractor, "$Revision: 1.51 $")
vtkInstantiatorNewMacro(gVTKRenderWindowInteractor)

#if defined(__WXGTK__) && defined(USE_WXGLCANVAS)
static int gVTK_attributes[]={
  WX_GL_DOUBLEBUFFER,
  WX_GL_RGBA,
  WX_GL_DEPTH_SIZE,
  16,
  0
};
#endif

//---------------------------------------------------------------------------
//#if defined(__WXGTK__) && defined(USE_WXGLCANVAS)
//gVTKRenderWindowInteractor::gVTKRenderWindowInteractor() :
//wxGLCanvas(0, -1, wxDefaultPosition, wxDefaultSize, 0, wxT("gVTKRenderWindowInteractor"), gVTK_attributes), vtkRenderWindowInteractor()
//#else
//gVTKRenderWindowInteractor::gVTKRenderWindowInteractor() : wxWindow(), vtkRenderWindowInteractor()
//#endif //__WXGTK__
gVTKRenderWindowInteractor::gVTKRenderWindowInteractor() :
    Control(),
     vtkRenderWindowInteractor()
      , timer()//timer(this, ID_gVTKRenderWindowInteractor_TIMER)
      , ActiveButton(0)//ActiveButton(wxEVT_NULL)
      , Stereo(0)
      , Handle(0)
      , Created(true)
      , RenderWhenDisabled(1)
      , UseCaptureMouse(0)
{
#ifdef VTK_DEBUG_LEAKS
  vtkDebugLeaks::ConstructClass("gVTKRenderWindowInteractor");
#endif
  this->RenderWindow = NULL;
  this->SetRenderWindow(vtkRenderWindow::New());
  this->RenderWindow->Delete();

  this->timer=memnew(Timer);
  timer->connect("timeout",this,"OnTimer");
}

//---------------------------------------------------------------------------
gVTKRenderWindowInteractor::gVTKRenderWindowInteractor(const HWND &parent,//wxWindow *parent,
                                                         int id//wxWindowID id,
                                                         /*,const POINT &pos,
                                                         const Vector2 &size,
                                                         long style,
                                                         const String &name*/)
//#if defined(__WXGTK__) && defined(USE_WXGLCANVAS)
//      : wxGLCanvas(parent, id, pos, size, style, name, gVTK_attributes), vtkRenderWindowInteractor()
//#else
//      : wxWindow(parent, id, pos, size, style, name), vtkRenderWindowInteractor()
//#endif //__WXGTK__
    : Control(),
     vtkRenderWindowInteractor()
     ,  timer()//timer(this, ID_gVTKRenderWindowInteractor_TIMER)
      , ActiveButton(0)//ActiveButton(wxEVT_NULL)
      , Stereo(0)
      , Handle(0)
      , Created(true)
      , RenderWhenDisabled(1)
      , UseCaptureMouse(0)
{
#ifdef VTK_DEBUG_LEAKS
  vtkDebugLeaks::ConstructClass("gVTKRenderWindowInteractor");
#endif
  this->RenderWindow = NULL;
  this->SetRenderWindow(vtkRenderWindow::New());
  this->RenderWindow->Delete();
#ifdef __WXMAC__
  // On Mac (Carbon) we don't get notified of the initial window size with an EVT_SIZE event,
  // so we update the size information of the interactor/renderwindow here
  this->UpdateSize(size.x, size.y);
#endif

  this->timer=memnew(Timer);
  timer->connect("timeout",this,"OnTimer");
  //add_child(timer);

  //LUCIANO
  timer->set_wait_time(0.01);
  timer->set_one_shot(true);
  timer->start();
  Enabled=true;

  Initialize();
  OnTimer();
  OnPaint();
}
//---------------------------------------------------------------------------
gVTKRenderWindowInteractor::~gVTKRenderWindowInteractor()
{
  SetRenderWindow(NULL);
  SetInteractorStyle(NULL);
}
//---------------------------------------------------------------------------
gVTKRenderWindowInteractor * gVTKRenderWindowInteractor::New()
{
  // we don't make use of the objectfactory, because we're not registered
  return new gVTKRenderWindowInteractor;
}
//---------------------------------------------------------------------------
//LUCIANO: vtkOverride
void gVTKRenderWindowInteractor::Initialize()
{
  int *size = RenderWindow->GetSize();
  // enable everything and start rendering
  Enable();
  //RenderWindow->Start();

  // set the size in the render window interactor
  //These are values of vtk
  Size[0] = size[0];
  Size[1] = size[1];

  // this is initialized
  Initialized = 1;
}
//---------------------------------------------------------------------------
void gVTKRenderWindowInteractor::Enable()
{
  // if already enabled then done
  if (Enabled)
    return;

  // that's it
  Enabled = 1;
#if defined(__WXGTK__) && defined(USE_WXGLCANVAS)
  SetCurrent();
#endif
  Modified();
}
//---------------------------------------------------------------------------
bool gVTKRenderWindowInteractor::Enable(bool enable)
{
//#if defined(__WXGTK__) && defined(USE_WXGLCANVAS)
//  return wxGLCanvas::Enable(enable);
//#else
//  return wxWindow::Enable(enable);
//#endif
//GODOT ENABLE CANVAS?
}
//---------------------------------------------------------------------------
void gVTKRenderWindowInteractor::Disable()
{
  // if already disabled then done
  if (!Enabled)
    return;

  // that's it (we can't remove the event handler like it should be...)
  Enabled = 0;
  Modified();
}
//---------------------------------------------------------------------------
void gVTKRenderWindowInteractor::Start()
{
  // the interactor cannot control the event loop
  vtkErrorMacro( << "gVTKRenderWindowInteractor::Start() "
    "interactor cannot control event loop.");
}
//---------------------------------------------------------------------------
void gVTKRenderWindowInteractor::UpdateSize(int x, int y)
{
  if( RenderWindow )
  {
    // if the size changed tell render window
    if ( x != Size[0] || y != Size[1] )
    {
      // adjust our (vtkRenderWindowInteractor size)
      Size[0] = x;
      Size[1] = y;
      // and our RenderWindow's size
      RenderWindow->SetSize(x, y);
#if defined(__WXMSW__)
      this->Refresh();
#endif //__WXMSW__
    }
  }
}
//---------------------------------------------------------------------------
int gVTKRenderWindowInteractor::CreateTimer(int timertype)
{
  // it's a one shot timer
  //if (!timer.start(10, TRUE)) //LUCIANO
  //if (!timer.start())
  //  return 0;

  timer->set_wait_time(0.01);
  timer->set_one_shot(true);
  timer->start();
  return 1;

}
#if VTK_MAJOR_VERSION > 5 || (VTK_MAJOR_VERSION == 5 && VTK_MINOR_VERSION >= 2)
//------------------------------------------------------------------
//int gVTKRenderWindowInteractor::InternalCreateTimer(int timerId, int timerType,
//                                                     unsigned long duration)
int gVTKRenderWindowInteractor::InternalCreateTimer(int timerId, int timerType,
                                                     unsigned long duration)
{
//  if (!timer.start(duration, timerType == OneShotTimer))
//  if (!timer.start)
//        return 0;
  timer->start();
  return ID_gVTKRenderWindowInteractor_TIMER;
}
//------------------------------------------------------------------
int gVTKRenderWindowInteractor::InternalDestroyTimer(int platformTimerId)
{
  timer->stop();
  return 1;
}
#endif
//---------------------------------------------------------------------------
int gVTKRenderWindowInteractor::DestroyTimer()
{
  // do nothing
  return 1;
}
//---------------------------------------------------------------------------

//void SpinBox::_range_click_timeout() {
//
//	if (!drag.enabled && Input::get_singleton()->is_mouse_button_pressed(BUTTON_LEFT)) {
//
//		bool up = get_local_mouse_pos().y < (get_size().height/2);
//		set_val( get_val() + (up?get_step():-get_step()));
//
//		if (range_click_timer->is_one_shot()) {
//			range_click_timer->set_wait_time(0.075);
//			range_click_timer->set_one_shot(false);
//			range_click_timer->start();
//		}
//
//	} else {
//		range_click_timer->stop();
//	}
//}
//In the original was
//EVT_TIMER       (ID_wxVTKRenderWindowInteractor_TIMER, wxVTKRenderWindowInteractor::OnTimer)
//And wxWidgets has a function named
void gVTKRenderWindowInteractor::OnTimer()
{
  if (!Enabled)
    return;

#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
  // new style
#if VTK_MAJOR_VERSION > 5 || (VTK_MAJOR_VERSION == 5 && VTK_MINOR_VERSION >= 2)
  // pass the right timer id
  int timerId = this->GetCurrentTimerId();
  this->InvokeEvent(vtkCommand::TimerEvent, &timerId);
#else
  this->InvokeEvent(vtkCommand::TimerEvent, NULL);
#endif
#else
  // old style
  InteractorStyle->OnTimer();
#endif
}

//---------------------------------------------------------------------------
// NOTE on implementation:
// Bad luck you ended up in the only tricky place of this code.
// A few note, wxWidgets still refuse to provide such convenient method
// so I have to maintain it myself, eventhough this is completely integrated
// in wxPython...
// Anyway if this happen to break for you then compare to a recent version of wxPython
// and look for the function long wxPyGetWinHandle(wxWindow* win)
// in wxPython/src/helpers.cpp
long gVTKRenderWindowInteractor::GetHandleHack()
{
  //helper function to hide the MSW vs GTK stuff
  long handle_tmp = 0;

// __WXMSW__ is for Win32
//__WXMAC__ stands for using Carbon C-headers, using either the CarbonLib/CFM or the native Mach-O builds (which then also use the latest features available)
// __WXGTK__ is for both gtk 1.2.x and gtk 2.x
#if defined(__WXMSW__) || defined(__WXMAC__)
    handle_tmp = (long)this->GetHandle();
#endif //__WXMSW__

//__WXCOCOA__ stands for using the objective-c Cocoa API
#ifdef __WXCOCOA__
   // Here is how to find the NSWindow
   wxTopLevelWindow* toplevel = dynamic_cast<wxTopLevelWindow*>(wxGetTopLevelParent( this ) );
   if (toplevel != NULL )
   {
    // AKT: use new Cocoa code if wxOSX 2.9.x
#if wxCHECK_VERSION(2, 9, 0)
    handle_tmp = (long)toplevel->GetWXWindow();
#else
    handle_tmp = (long)toplevel->GetNSWindow();
#endif
   }
   // The NSView will be deducted from
   // [(NSWindow*)Handle contentView]
   // if only I knew how to write that in c++
#endif //__WXCOCOA__

    // Find and return the actual X-Window.
#if defined(__WXGTK__) || defined(__WXX11__)
    return (long)GetXWindow(this);
#endif

//#ifdef __WXMOTIF__
//    handle_tmp = (long)this->GetXWindow();
//#endif

  return handle_tmp;
}
//---------------------------------------------------------------------------
//void gVTKRenderWindowInteractor::OnPaint(wxPaintEvent& WXUNUSED(event))
void gVTKRenderWindowInteractor::OnPaint()
{
  //must always be here
  //wxPaintDC pDC(this);

  //do it here rather than in the cstor: this is safer.
  if(!Handle)
  {
    Handle = GetHandleHack(); //Funcion de VTK, clave
    RenderWindow->SetWindowId(reinterpret_cast<void *>(Handle));
// Cocoa
// this->GetNSView() <-> DisplayId
// this->GetTopLevel()->GetNSWindow() <-> WindowId
#ifdef __WXMSW__
    RenderWindow->SetParentId(reinterpret_cast<void *>(this->GetParent()->GetHWND()));
#endif //__WXMSW__

    // This is another hack to prevent the VTK Render Window from closing the display.
    // If VTK closes the display, ~wxContext chashes while trying to destroy its
    // glContext (because the display is closed). The Get -> Set makes this VTK
    // object think someone else is responsible for the display.
    this->RenderWindow->SetDisplayId(this->RenderWindow->GetGenericDisplayId());
  }
  // get vtk to render to the wxWindows
  Render();
#ifdef __WXMAC__
  // This solves a problem with repainting after a window resize
  // See also: http://sourceforge.net/mailarchive/forum.php?thread_id=31690967&forum_id=41789
#ifdef __WXCOCOA__
  vtkCocoaRenderWindow * rwin = vtkCocoaRenderWindow::SafeDownCast(RenderWindow);
  if( rwin )
  {
    rwin->UpdateContext();
  }
#else
  vtkCarbonRenderWindow* rwin = vtkCarbonRenderWindow::SafeDownCast(RenderWindow);
  if( rwin )
  {
#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 4)
    // Must be somewhere after VTK 4.4
    rwin->UpdateGLRegion();
#endif
  }
#endif
#endif
}
//---------------------------------------------------------------------------
void gVTKRenderWindowInteractor::OnEraseBackground()
{
  //turn off background erase to reduce flickering on MSW
//  event.Skip(false);
}
//---------------------------------------------------------------------------
void gVTKRenderWindowInteractor::OnSize()
{
//  int w, h;
//  GetClientSize(&w, &h);
//  UpdateSize(w, h);
//
//  if (!Enabled)
//    {
//    return;
//    }
//
//#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
//  InvokeEvent(vtkCommand::ConfigureEvent, NULL);
//#endif
//  //this will check for Handle
//  //Render();
}
//---------------------------------------------------------------------------
void gVTKRenderWindowInteractor::OnMotion(InputEventMouseMotion &event)
{
// if (!Enabled)
//    {
//    return;
//    }
//#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
//  SetEventInformationFlipY(event.GetX(), event.GetY(),
//    event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);
//
//  InvokeEvent(vtkCommand::MouseMoveEvent, NULL);
//#else
//  InteractorStyle->OnMouseMove(event.ControlDown(), event.ShiftDown(),
//    event.GetX(), Size[1] - event.GetY() - 1);
//#endif
}
//---------------------------------------------------------------------------
#if !(VTK_MAJOR_VERSION == 3 && VTK_MINOR_VERSION == 1)
//void gVTKRenderWindowInteractor::OnEnter(wxMouseEvent &event)
//{
//  if (!Enabled)
//    {
//    return;
//    }
//
//#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
//    // new style
//  SetEventInformationFlipY(event.GetX(), event.GetY(),
//      event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);
//
//  InvokeEvent(vtkCommand::EnterEvent, NULL);
//#else
//    // old style
//  InteractorStyle->OnEnter(event.ControlDown(), event.ShiftDown(),
//      event.GetX(), Size[1] - event.GetY() - 1);
//#endif
//}
////---------------------------------------------------------------------------
//void gVTKRenderWindowInteractor::OnLeave(wxMouseEvent &event)
//{
//  if (!Enabled)
//    {
//    return;
//    }
//
//#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
//    // new style
//  SetEventInformationFlipY(event.GetX(), event.GetY(),
//      event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);
//
//  InvokeEvent(vtkCommand::LeaveEvent, NULL);
//#else
//    // old style
//  InteractorStyle->OnLeave(event.ControlDown(), event.ShiftDown(),
//      event.GetX(), Size[1] - event.GetY() - 1);
//#endif
//}
//---------------------------------------------------------------------------
void gVTKRenderWindowInteractor::OnKeyDown(InputEventKey &event)
{
//  if (!Enabled)
//    {
//    return;
//    }
//
//#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
//    // new style
//  int keycode = event.GetKeyCode();
//  char key = '\0';
//  if (((unsigned int)keycode) < 256)
//  {
//    // TODO: Unicode in non-Unicode mode ??
//    key = (char)keycode;
//  }
//
//  // we don't get a valid mouse position inside the key event on every platform
//  // so we retrieve the mouse position explicitly and pass it along
//  wxPoint mousePos = ScreenToClient(wxGetMousePosition());
//  SetEventInformationFlipY(mousePos.x, mousePos.y,
//                           event.ControlDown(), event.ShiftDown(), key, 0, NULL);
//  InvokeEvent(vtkCommand::KeyPressEvent, NULL);
//#else
//  InteractorStyle->OnKeyDown(event.ControlDown(), event.ShiftDown(),
//    event.GetKeyCode(), 1);
//#endif
//  //event.Skip(); //LUCIANOs
}
//---------------------------------------------------------------------------
void gVTKRenderWindowInteractor::OnKeyUp(InputEventKey &event)
{
//  if (!Enabled)
//    {
//    return;
//    }
//
//#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
//    // new style
//  int keycode = event.GetKeyCode();
//  char key = '\0';
//  if (((unsigned int)keycode) < 256)
//  {
//    // TODO: Unicode in non-Unicode mode ??
//    key = (char)keycode;
//  }
//
//  // we don't get a valid mouse position inside the key event on every platform
//  // so we retrieve the mouse position explicitly and pass it along
//  wxPoint mousePos = ScreenToClient(wxGetMousePosition());
//  LIke in GODOT //LUCIANO
//  POINT mousePos
//  ScreenToClient(hWnd, &mousePos) //LUCIANO
//  SetEventInformationFlipY(mousePos.x, mousePos.y,
//                           event.ControlDown(), event.ShiftDown(), key, 0, NULL);
//  InvokeEvent(vtkCommand::KeyReleaseEvent, NULL);
//#else
//  InteractorStyle->OnKeyUp(event.ControlDown(), event.ShiftDown(),
//    event.GetKeyCode(), 1);
//#endif
//  event.Skip();
}
#endif //!(VTK_MAJOR_VERSION == 3 && VTK_MINOR_VERSION == 1)
 //---------------------------------------------------------------------------
void gVTKRenderWindowInteractor::OnChar(InputEventKey &event)
{
//  if (!Enabled)
//    {
//    return;
//    }
//
//#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
//  // new style
//  int keycode = event.GetKeyCode();
//  char key = '\0';
//  if (((unsigned int)keycode) < 256)
//    {
//    // TODO: Unicode in non-Unicode mode ??
//    key = (char)keycode;
//    }
//
//  // we don't get a valid mouse position inside the key event on every platform
//  // so we retrieve the mouse position explicitly and pass it along
//  wxPoint mousePos = ScreenToClient(wxGetMousePosition());
//  SetEventInformationFlipY(mousePos.x, mousePos.y,
//                           event.ControlDown(), event.ShiftDown(), key, 0, NULL);
//  InvokeEvent(vtkCommand::CharEvent, NULL);
//#endif
//  //event.Skip();
}
//---------------------------------------------------------------------------
void gVTKRenderWindowInteractor::OnButtonDown(InputEventMouseButton &event)
{
//  if (!Enabled || (ActiveButton != wxEVT_NULL))
//    {
//    return;
//    }
//  ActiveButton = event.GetEventType();
//
//    // On Mac (Carbon) and Windows we don't automatically get the focus when
//    // you click inside the window
//    // we therefore set the focus explicitly
//    // Apparently we need that on linux (GTK) too:
//    this->SetFocus();
//
//#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
//  SetEventInformationFlipY(event.GetX(), event.GetY(),
//    event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);
//#endif
//
//  if(event.RightDown())
//  {
//#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
//    // new style
//    InvokeEvent(vtkCommand::RightButtonPressEvent, NULL);
//#else
//    // old style
//    InteractorStyle->OnRightButtonDown(event.ControlDown(), event.ShiftDown(),
//      event.GetX(), Size[1] - event.GetY() - 1);
//#endif
//  }
//  else if(event.LeftDown())
//  {
//#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
//    // new style
//    InvokeEvent(vtkCommand::LeftButtonPressEvent, NULL);
//#else
//    // old style
//    InteractorStyle->OnLeftButtonDown(event.ControlDown(),  event.ShiftDown(),
//      event.GetX(), Size[1] - event.GetY() - 1);
//#endif
//  }
//  else if(event.MiddleDown())
//  {
//#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
//    // new style
//    InvokeEvent(vtkCommand::MiddleButtonPressEvent, NULL);
//#else
//    // old style
//    InteractorStyle->OnMiddleButtonDown(event.ControlDown(), event.ShiftDown(),
//      event.GetX(), Size[1] - event.GetY() - 1);
//#endif
//  }
//  //save the button and capture mouse until the button is released
//  //Only if :
//  //1. it is possible (WX_USE_X_CAPTURE)
//  //2. user decided to.
//  if ((ActiveButton != wxEVT_NULL) && WX_USE_X_CAPTURE && UseCaptureMouse)
//  {
//    CaptureMouse();
//  }
}
//---------------------------------------------------------------------------
void gVTKRenderWindowInteractor::OnButtonUp(InputEventMouseButton &event)
{
//  //EVT_xxx_DOWN == EVT_xxx_UP - 1
//  //This is only needed if two mouse buttons are pressed at the same time.
//  //In wxWindows 2.4 and later: better use of wxMOUSE_BTN_RIGHT or
//  //wxEVT_COMMAND_RIGHT_CLICK
//  if (!Enabled || (ActiveButton != (event.GetEventType()-1)))
//    {
//    return;
//    }
//
//  // See report by Shang Mu / Kerry Loux on gVTK mailing list
//    this->SetFocus();
//
//#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
//  SetEventInformationFlipY(event.GetX(), event.GetY(),
//    event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);
//#endif
//
//  if(ActiveButton == wxEVT_RIGHT_DOWN)
//  {
//#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
//    // new style
//    InvokeEvent(vtkCommand::RightButtonReleaseEvent, NULL);
//#else
//    // old style
//    InteractorStyle->OnRightButtonUp(event.ControlDown(), event.ShiftDown(),
//      event.GetX(), Size[1] - event.GetY() - 1);
//#endif
//  }
//  else if(ActiveButton == wxEVT_LEFT_DOWN)
//  {
//#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
//    // new style
//    InvokeEvent(vtkCommand::LeftButtonReleaseEvent, NULL);
//#else
//    // old style
//    InteractorStyle->OnLeftButtonUp(event.ControlDown(), event.ShiftDown(),
//      event.GetX(), Size[1] - event.GetY() - 1);
//#endif
//  }
//  else if(ActiveButton == wxEVT_MIDDLE_DOWN)
//  {
//#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
//    // new style
//    InvokeEvent(vtkCommand::MiddleButtonReleaseEvent, NULL);
//#else
//    // old style
//    InteractorStyle->OnMiddleButtonUp(event.ControlDown(), event.ShiftDown(),
//      event.GetX(), Size[1] - event.GetY() - 1);
//#endif
//  }
//  //if the ActiveButton is realeased, then release mouse capture
//  if ((ActiveButton != wxEVT_NULL) && WX_USE_X_CAPTURE && UseCaptureMouse)
//  {
//    ReleaseMouse();
//  }
//  ActiveButton = wxEVT_NULL;
}
//---------------------------------------------------------------------------
void gVTKRenderWindowInteractor::OnMouseWheel(InputEventMouseButton& event)
{
//// Mouse wheel was only added after VTK 4.4 (I think...)
//#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 4)
//  // new style
//  //Set vtk event information ... The numebr of wheel rotations is stored in
//  //the x varible.  y varible is zero
//  SetEventInformationFlipY(event.GetX() , event.GetY(),
//                           event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);
//  if(event.GetWheelRotation() > 0)
//    {
//      //Send event to VTK
//      InvokeEvent(vtkCommand::MouseWheelForwardEvent, NULL);
//    }
//  else
//    {
//      //Send event to VTK
//      InvokeEvent(vtkCommand::MouseWheelBackwardEvent, NULL);
//    }
//#endif

}

//---------------------------------------------------------------------------
//#if wxCHECK_VERSION(2, 8, 0)
//void gVTKRenderWindowInteractor::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
//{
//   if (ActiveButton != wxEVT_NULL)
//   {
//       //Maybe also invoke the button release event here
//   }
//   // Reset ActiveButton so that
//   // 1. we do not process mouse button up events any more,
//   // 2. the next button down event will be processed and call CaptureMouse().
//   // Otherwise ReleaseMouse() will be called
//   // without a previous CaptureMouse().
//   ActiveButton = wxEVT_NULL;
//}
//#endif

//---------------------------------------------------------------------------
void gVTKRenderWindowInteractor::Render()
{
////#if wxCHECK_VERSION(2, 8, 0)
//  int renderAllowed = !IsFrozen();
////#else
////  int renderAllowed = 1;
////#endif
//  if (renderAllowed && !RenderWhenDisabled)
//    {
//    //the user doesn't want us to render when the toplevel frame
//    //is disabled - first find the top level parent
//    wxWindow *topParent = wxGetTopLevelParent(this);
//    if (topParent)
//      {
//      //if it exists, check whether it's enabled
//      //if it's not enabeld, renderAllowed will be false
//      renderAllowed = topParent->IsEnabled();
//      }
//    }
  bool renderAllowed=true; //LUCIANO
  if (renderAllowed)
    {
    if(Handle && (Handle == GetHandleHack()) )
      {
      RenderWindow->Render();
      }
#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 2)
    else if(GetHandleHack())
      {
      //this means the user has reparented us; let's adapt to the
      //new situation by doing the WindowRemap dance
      //store the new situation
      Handle = GetHandleHack();
      RenderWindow->SetNextWindowId(reinterpret_cast<void *>(Handle));
      RenderWindow->WindowRemap();
      RenderWindow->Render();
      }
#endif
    }
}
//---------------------------------------------------------------------------
void gVTKRenderWindowInteractor::SetRenderWhenDisabled(int newValue)
{
  //Change value of __RenderWhenDisabled ivar.
  //If __RenderWhenDisabled is false (the default), this widget will not
  //call Render() on the RenderWindow if the top level frame (i.e. the
  //containing frame) has been disabled.

  //This prevents recursive rendering during wxSafeYield() calls.
  //wxSafeYield() can be called during the ProgressMethod() callback of
  //a VTK object to have progress bars and other GUI elements updated -
  //it does this by disabling all windows (disallowing user-input to
  //prevent re-entrancy of code) and then handling all outstanding
  //GUI events.

  //However, this often triggers an OnPaint() method for gVTKRWIs,
  //resulting in a Render(), resulting in Update() being called whilst
  //still in progress.

  RenderWhenDisabled = (bool)newValue;
}
//---------------------------------------------------------------------------
//
// Set the variable that indicates that we want a stereo capable window
// be created. This method can only be called before a window is realized.
//
void gVTKRenderWindowInteractor::SetStereo(int capable)
{
  if (Stereo != capable)
    {
    Stereo = capable;
    RenderWindow->StereoCapableWindowOn();
    RenderWindow->SetStereoTypeToCrystalEyes();
    Modified();
    }
}

//---------------------------------------------------------------------------
//
//
void gVTKRenderWindowInteractor::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

