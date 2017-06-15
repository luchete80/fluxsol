// ImGui - standalone example application for Glfw + OpenGL 2, using fixed pipeline
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include <stdio.h>
#include <GLFW/glfw3.h>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkConeSource.h>
#include <vtkSuperquadricSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>



#include "Timer.h"
#include <windows.h>


#include "vtkImRenderWindowInteractor.h"

#include <iostream>
#include <ctime>
#include <cstdlib>

void ShowMainWindow();


void create_cone_pipeline(vtkImRenderWindowInteractor *flrwi)
{
  // create a rendering window and renderer
  vtkRenderer *ren = vtkRenderer::New();
  ren->SetBackground(0.5,0.5,0.5);

  vtkRenderWindow *renWindow = vtkRenderWindow::New();
  renWindow->AddRenderer(ren);
  // uncomment the statement below if things aren't rendering 100% on your
  // configuration; the debug output could give you clues as to why
  //renWindow->DebugOn();

  // NB: here we treat the vtkFlRenderWindowInteractor just like any other
  // old vtkRenderWindowInteractor
//  flrwi->SetRenderWindow(renWindow);
  // just like with any other vtkRenderWindowInteractor(), you HAVE to call
  // Initialize() before the interactor will function.  See the docs in
  // vtkRenderWindowInteractor.h
//  flrwi->Initialize();

  // create an actor and give it cone geometry
//  vtkConeSource *cone = vtkConeSource::New();
//  cone->SetResolution(8);
//  vtkPolyDataMapper *coneMapper = vtkPolyDataMapper::New();
//  coneMapper->SetInputConnection(cone->GetOutputPort());
//  vtkActor *coneActor = vtkActor::New();
//  coneActor->SetMapper(coneMapper);
//  coneActor->GetProperty()->SetColor(1.0, 0.0, 1.0);
//
//  // assign our actor to the renderer
//  ren->AddActor(coneActor);
//
//  // We can now delete all our references to the VTK pipeline (except for
//  // our reference to the vtkFlRenderWindowInteractor) as the objects
//  // themselves will stick around until we dereference fl_vtk_window
//  ren->Delete();
//  renWindow->Delete();
//  cone->Delete();
//  coneMapper->Delete();
//  coneActor->Delete();
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}



int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui OpenGL2 example", NULL, NULL);
    glfwMakeContextCurrent(window);

    // Setup ImGui binding
    ImGui_ImplGlfw_Init(window, true);


    // Load Fonts
    // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
    //ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

    bool show_test_window = true;
    bool show_another_window = false;
	bool show_model_window=true;
	bool show_menu_window=true;
    ImVec4 clear_color = ImColor(114, 144, 154);


    //////////////////
    /// LUCIANO: BEFORE MAIN LOOP
//    vtkImRenderWindowInteractor *im_vtk_window = NULL;
//    im_vtk_window=vtkImRenderWindowInteractor::New();
//
//    HINSTANCE hinst;
//    im_vtk_window->addInstance(&hinst);
//    //create_cone_pipeline(im_vtk_window);
    //im_vtk_window->CreateTimer(VTKI_TIMER_FIRST);
    //im_vtk_window->Start();

    //For timers


//    clock_t startTime = clock(); //Start timer
//    double secondsPassed;
//    double secondsToDelay = 0.1;
//    std::cout << "Time to delay: " << secondsToDelay << std::endl;

    ///////////////////////// FIN LUCIANO


    // Main loop
    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();
        ImGui_ImplGlfw_NewFrame();


		ShowMainWindow();


		ImGui::SetNextWindowSize(ImVec2(350,560), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Model", &show_model_window);
		ImVec2 size = ImGui::GetContentRegionAvail();
        // ImGui::Image(&m_texture_handle, size, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();


        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        glfwSwapBuffers(window);

//        secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
//        if(secondsPassed >= secondsToDelay)
//        {
//            startTime=clock();
//            im_vtk_window->Render();
//            im_vtk_window->OnTimer();
//            //im_vtk_window->draw();
//            cout << "timeout"<<endl;
//        }

        //im_vtk_window->Render();
    }

    // Cleanup
    ImGui_ImplGlfw_Shutdown();
    glfwTerminate();

    //im_vtk_window=NULL; //If not put this is crashing

    return 0;
}

// En void SceneView::onGUI()

//According with Lumix Engine
//		auto size = ImGui::GetContentRegionAvail();
//		auto* fb = m_pipeline->getFramebuffer("default");
// m_texture_handle = fb->getRenderbufferHandle(0);
// ImGui::Image(&m_texture_handle, size, ImVec2(0, 1), ImVec2(1, 0));
// m_pipeline->render();

//http://public.kitware.com/pipermail/vtkusers/2009-May/051890.html
//
//Hello,
//
//We want to render multiple renderscreen with help of different camera angles and save them in different vtkTextures. We managed it, to render them and save them on the harddisk. But in the moment we render them, and give these rendered screens different vtkTextures, the last in vtkTextures rendered screen, override all other prerendered vtkTextures. For the renderings we use the class vtkWindowToImageFilter. Some code:
//
//thanks
//maxim
//
//code:
////x,y,z direction of view
//vtkTexture* getImageFromWindow(vtkRenderer *ren,vtkRenderWindow *win, vtkActor *ac, double x, double y, double z)
//{
//vtkCamera *c= vtkCamera::New();
//double *richt = ac->GetCenter();
//c->SetPosition(richt[0],richt[1],richt[2]);
//c->SetFocalPoint(richt[0]+x,richt[1]+y,richt[2]+z);
//ren->SetActiveCamera(c);
//
//win->Render();
//
//vtkWindowToImageFilter *wti= vtkWindowToImageFilter::New();
//wti->SetInput(win);
//
//vtkTexture *t = vtkTexture::New();
//t->SetInput( wti->GetOutput());
//t->InterpolateOn();
//return t;
//}
//
////use of the function:
//vtkTexture *id2 = vtkTexture::New();
//id2 = getImageFromWindow(renderer,renWin,actor,1,0,0);
//vtkTexture *id1 = vtkTexture::New();
//id1 = getImageFromWindow(renderer,renWin,actor,-1,0,0);
//
//actor->SetTexture(id2);
//
////actor don't get the texture from id2, but the actor get the texture from id1. although id2 was rendered first and was given to the actor.
//thanks for any suggestions.
//maxim

void ShowMainWindow()
{
    bool p_open=true;

    static bool no_titlebar = false;
    static bool no_border = true;
    static bool no_resize = false;
    static bool no_move = false;
    static bool no_scrollbar = false;
    static bool no_collapse = false;
    static bool no_menu = false;

    // Demonstrate the various window flags. Typically you would just use the default.
    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
    if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
    if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
    if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
    if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;

        ImGui::SetNextWindowSize(ImVec2(550,680), ImGuiSetCond_FirstUseEver);
        ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

       if (!ImGui::Begin("ImGui Demo", &p_open,window_flags))
       {
            ImGui::End();
            return;
       }
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                    ImGui::MenuItem("(dummy menu)", NULL, false, false);
                    if (ImGui::MenuItem("New")) {}
                    if (ImGui::MenuItem("Open Model", "Ctrl+O")) {}
                    if (ImGui::MenuItem("Import Mesh", "Ctrl+M")) {}
                //ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::End();
}
