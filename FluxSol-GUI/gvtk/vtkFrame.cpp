#include "vtkFrame.h"

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_Quit = 1,
    Minimal_About
};

#define MY_FRAME      101
#define MY_VTK_WINDOW 102
// LUCIANO: HERE IS THE MAIN AND THE TABLE

//// 'Main program' equivalent: the program execution "starts" here
//bool MyApp::OnInit()
//{
//    // create the main application window
//    MyFrame *frame = new MyFrame(_T("wxWindows-VTK App"),
//                                 wxPoint(50, 50), wxSize(450, 340));
//
//    // and show it (the frames, unlike simple controls, are not shown when
//    // created initially)
//    frame->Show(TRUE);
//
//    // success: wxApp::OnRun() will be called which will enter the main message
//    // loop and the application will run. If we returned FALSE here, the
//    // application would exit immediately.
//    return TRUE;
//}

// frame constructor
MyFrame::MyFrame(HWND hWnd) //the parent
{
// #ifdef __WXMAC__
    // // we need this in order to allow the about menu relocation, since ABOUT is
    // // not the default id of the about menu
    // wxApp::s_macAboutMenuItemId = Minimal_About;
// #endif

//    // set the frame icon
//    SetIcon(wxICON(mondrian));
//
//    // create a menu bar
//    wxMenu *menuFile = new wxMenu(_T(""), wxMENU_TEAROFF);
//
//    // the "About" item should be in the help menu
//    wxMenu *helpMenu = new wxMenu;
//    helpMenu->Append(Minimal_About, _T("&About...\tCtrl-A"), _T("Show about dialog"));
//
//    menuFile->Append(Minimal_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));
//
//    // now append the freshly created menu to the menu bar...
//    wxMenuBar *menuBar = new wxMenuBar();
//    menuBar->Append(menuFile, _T("&File"));
//    menuBar->Append(helpMenu, _T("&Help"));
//
//    // ... and attach this menu bar to the frame
//    SetMenuBar(menuBar);
//
//#if wxUSE_STATUSBAR
//    // create a status bar just for fun (by default with 1 pane only)
//    CreateStatusBar(2);
//    SetStatusText(_T("Drag the mouse here! (wxWindows 2.4.0)"));
//#endif // wxUSE_STATUSBAR
	HINSTANCE vtk_hinstance;
    vtkwin = CreateWindow ( "Test",
                       "Draw Window",
                       WS_CHILD | WS_VISIBLE | SS_CENTER,
                         100,100, //xy pos
                       400,
                       480,
                       hWnd,
                       (HMENU)2,
                       vtk_hinstance,
                       NULL);

    m_pVTKWindow = new gVTKRenderWindowInteractor();//(this, MY_VTK_WINDOW);
    m_pVTKWindow->UseCaptureMouseOn();
//    m_pVTKWindow->DebugOn();
    ConstructVTK();
    ConfigureVTK();

    ShowWindow(hWnd,SW_SHOW);   //LUCIANO
}

MyFrame::~MyFrame()
{
  if(m_pVTKWindow) m_pVTKWindow->Delete();
  DestroyVTK();
}

void MyFrame::ConstructVTK()
{
  pRenderer     = vtkRenderer::New();
  pConeMapper   = vtkPolyDataMapper::New();
  pConeActor    = vtkActor::New();
  pConeSource   = vtkConeSource::New();

  pConeSource->Update(); //LUCIANO

}

void MyFrame::ConfigureVTK()
{
  // connect the render window and wxVTK window
  pRenderWindow = m_pVTKWindow->GetRenderWindow();

  // connect renderer and render window and configure render window
  pRenderWindow->AddRenderer(pRenderer);

  // initialize cone
  pConeSource->SetResolution(8);

  // connect pipeline
  //pConeMapper->SetInput(pConeSource->GetOutput()); //LUCIANO
  //pConeMapper->SetInputData(pConeSource->GetOutput()); //LUCIANO
  pConeMapper->SetInputConnection(pConeSource->GetOutputPort());
  pConeActor->SetMapper(pConeMapper);
  pRenderer->AddActor(pConeActor);

  // configure renderer
  pRenderer->SetBackground(1.0,0.333333,0.5);
  pRenderer->GetActiveCamera()->Elevation(30.0);
  pRenderer->GetActiveCamera()->Azimuth(30.0);
  pRenderer->GetActiveCamera()->Zoom(1.0);
  pRenderer->GetActiveCamera()->SetClippingRange(1,1000);

  //pRenderer->Render();//LUCIANO

}

void MyFrame::DestroyVTK()
{
//http://www.vtk.org/pipermail/vtkusers/2003-September/019894.html
  if (pRenderer != 0)
    pRenderer->Delete();
  if (pConeMapper != 0)
    pConeMapper->Delete();
  if (pConeActor != 0)
    pConeActor->Delete();
  if (pConeSource != 0)
    pConeSource->Delete();
}

// event handlers

// void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
// {
    // // TRUE is to force the frame to close
    // Close(TRUE);
// }

// void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
// {
    // wxString msg;
    // msg.Printf( _T("This is the about dialog of wx-vtk sample.\n"));

    // wxMessageBox(msg, _T("About wx-vtk"), wxOK | wxICON_INFORMATION, this);
// }
