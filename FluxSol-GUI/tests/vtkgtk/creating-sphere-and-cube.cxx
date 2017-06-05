//======================================================================
//  An example file showing how to use the vtkGtkRenderWindowInteractor.
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Tue 2008-03-11 21:05 
//----------------------------------------------------------------------
#include "vtkSphereSource.h"
#include "vtkCubeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkGtkRenderWindowInteractor.h"
#include "vtkInteractorStyleFlight.h"
#include "gtk/gtkgl.h"

// Creates a scene consisting of a sphere and a 3D block
vtkRenderer *create_scene()
{
  // create sphere geometry
  vtkSphereSource *sphere = vtkSphereSource::New();
  sphere->SetRadius(1.0);
  sphere->SetThetaResolution(50);
  sphere->SetPhiResolution(50);

  // map to graphics library
  vtkPolyDataMapper *sphere_map = vtkPolyDataMapper::New();
  sphere_map->SetInput(sphere->GetOutput());

  // actor coordinates geometry, properties, transformation
  vtkActor *sphere_actor = vtkActor::New();
  sphere_actor->SetMapper(sphere_map);
  sphere_actor->GetProperty()->SetColor(0,0,1); // sphere color blue

  // cube
  vtkCubeSource *cube = vtkCubeSource::New();
  cube->SetXLength(1.5);
  cube->SetYLength(1.0);
  cube->SetZLength(0.5);

  // map to graphics library
  vtkPolyDataMapper *cube_map = vtkPolyDataMapper::New();
  cube_map->SetInput(cube->GetOutput());

  // actor coordinates geometry, properties, transformation
  vtkActor *cube_actor = vtkActor::New();
  cube_actor->SetMapper(cube_map);
  cube_actor->SetPosition(2.5,0,0);
  cube_actor->GetProperty()->SetColor(1,0,0); // cube color is red

  // a renderer and and place the objects in it render window
  vtkRenderer *ren1 = vtkRenderer::New();
  ren1->AddActor(sphere_actor);
  ren1->AddActor(cube_actor);
  ren1->SetBackground(1,1,1); // Background color white

  return ren1;
}

// Create's the gui and incorpotes the drawing area widget of
// the renderer.
void build_gui(GtkWidget *vtk_area)
{
  // Build gui
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget *vbox = gtk_vbox_new(0,0);

  gtk_window_set_policy(GTK_WINDOW(window), TRUE, TRUE, TRUE);

  gtk_container_add (GTK_CONTAINER (window),
		     vbox);
  gtk_box_pack_start (GTK_BOX (vbox),
		      vtk_area, 1,1,0);
  gtk_widget_set_size_request(vtk_area, 400,400);
  GtkWidget *button = gtk_button_new_with_label("Quit");
  g_signal_connect(G_OBJECT(button), "clicked",
		   G_CALLBACK(gtk_main_quit), NULL);
  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE,FALSE,0);
 
  gtk_widget_show_all (window);

}

int main(int argc, char *argv[])
{
  gtk_init(&argc, &argv);
  gtk_gl_init(&argc, &argv);

  // Create the gtk vtk interactor. This also creates the gtk widget.
  vtkGtkRenderWindowInteractor *iren = vtkGtkRenderWindowInteractor::New();

  // Connect the renderer to the gtk vtk rendering window
  vtkRenderer *scene = create_scene();

  iren->GetRenderWindow()->AddRenderer(scene);

  build_gui(iren->get_drawing_area());

  gtk_main();
}
