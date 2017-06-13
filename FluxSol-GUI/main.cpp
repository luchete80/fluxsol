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



#include "Timer_win32.h"


#include "vtkImRenderWindowInteractor.h"



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
  flrwi->SetRenderWindow(renWindow);
  // just like with any other vtkRenderWindowInteractor(), you HAVE to call
  // Initialize() before the interactor will function.  See the docs in
  // vtkRenderWindowInteractor.h
  flrwi->Initialize();

  // create an actor and give it cone geometry
  vtkConeSource *cone = vtkConeSource::New();
  cone->SetResolution(8);
  vtkPolyDataMapper *coneMapper = vtkPolyDataMapper::New();
  coneMapper->SetInputConnection(cone->GetOutputPort());
  vtkActor *coneActor = vtkActor::New();
  coneActor->SetMapper(coneMapper);
  coneActor->GetProperty()->SetColor(1.0, 0.0, 1.0);

  // assign our actor to the renderer
  ren->AddActor(coneActor);

  // We can now delete all our references to the VTK pipeline (except for
  // our reference to the vtkFlRenderWindowInteractor) as the objects
  // themselves will stick around until we dereference fl_vtk_window
  ren->Delete();
  renWindow->Delete();
  cone->Delete();
  coneMapper->Delete();
  coneActor->Delete();
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
    ImVec4 clear_color = ImColor(114, 144, 154);


    //////////////////
    /// LUCIANO: BEFORE MAIN LOOP
    vtkImRenderWindowInteractor *im_vtk_window = NULL;
    im_vtk_window=vtkImRenderWindowInteractor::New();
    //create_cone_pipeline(im_vtk_window);

    //For timers

    HINSTANCE hinst;
    fl_display=hinst;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();
        ImGui_ImplGlfw_NewFrame();

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        {
            static float f = 0.0f;
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if (ImGui::Button("Test Window")) show_test_window ^= 1;
            if (ImGui::Button("Another Window")) show_another_window ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // 2. Show another simple window, this time using an explicit Begin/End pair
        if (show_another_window)
        {
            ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello");
            ImGui::End();
        }

        // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        if (show_test_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }

		/////////////////////////////////////////////
		//Test Luciano Window
		// NUEVOOOOOOOOOOOOOOOOOOOOOOOOOOOO
		////////////////////////////////////////////

		//After whowing and all


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

        //im_vtk_window->Render();
    }

    // Cleanup
    ImGui_ImplGlfw_Shutdown();
    glfwTerminate();

    im_vtk_window=NULL; //If not put this is crashing

    return 0;
}

// En void SceneView::onGUI()

//According with Lumix Engine
//		auto size = ImGui::GetContentRegionAvail();
//		auto* fb = m_pipeline->getFramebuffer("default");
// m_texture_handle = fb->getRenderbufferHandle(0);
// ImGui::Image(&m_texture_handle, size, ImVec2(0, 1), ImVec2(1, 0));
// m_pipeline->render();
