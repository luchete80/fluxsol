//http://andrewd.ces.clemson.edu/courses/graphics/examples/gtk-simple/gtkglsimple.c

#include	<stdio.h>
#include	<stdlib.h>
#include	<gdk/gdk.h>
#include	<gdk/gdkkeysyms.h>
#include	<gtk/gtk.h>
#include	<gtk/gtkwidget.h>
#include	<gtk/gtkglarea.h>
#include	<GL/gl.h>
#include	<GL/glu.h>

/* Prototypes */

int		main(int, char **);
GtkWidget	*create_ctl_window();
GtkWidget	*create_gl_window();
void		add_button(GtkWidget *);
gint		destroyapp(GtkWidget *, gpointer);
gint		closeAppWindow(GtkWidget *, gpointer);
void		button_was_clicked(GtkWidget *, gpointer);
void		initgl(void);
gint		glarea_configure(GtkWidget *widget, GdkEventConfigure *event);
gint		glarea_expose(GtkWidget *widget, GdkEventExpose *event);
gint		glarea_motion_notify(GtkWidget *widget, GdkEventMotion *event);
gint		glarea_button_press(GtkWidget *widget, GdkEventButton *event);
gint		glarea_key_press_event(GtkWidget *widget, GdkEventKey *event);

/* Globals */
GtkWidget	*glarea=NULL;

gint closeAppWindow(GtkWidget *widget, gpointer gdata)
{
  g_print("Quitting...\n");
  gtk_main_quit();

  /* ... Continue closing ... */
  return(FALSE);
}

void button_was_clicked(GtkWidget *widget, gpointer gdata)
{
  g_print("Button was clicked.\n");
}

gint destroyapp(GtkWidget *widget, gpointer gdata)
{
  g_print("Quitting...\n");
  gtk_main_quit();

  /* ... Ok to be closed.  TRUE would prevent window from closing ... */
  return(FALSE);
}

void initgl(void)
{
  /* ... Specify clear color (black) ... */
  glClearColor(0.0,0.0,0.0,0.0);

  /* ... Turn on depth testing (z-buffering) ... */
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0);

  /* ... Turn on blending (for transparency) ... */
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

gint glarea_expose(GtkWidget *widget, GdkEventExpose *event)
{
  /* ... Draw only last expose ... */
  if(event->count > 0) {
    return TRUE;
  }

  gtk_widget_grab_focus(GTK_WIDGET(glarea));

  /* ... OpenGL calls can be done only if begingl returns true ... */
  if (gtk_gl_area_begingl(GTK_GL_AREA(glarea))) {

    /* ... View ... */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,glarea->allocation.width,0.0,glarea->allocation.height);

    glDrawBuffer(GL_FRONT_AND_BACK);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* ... DRAW STUFF HERE ... */
    glDrawBuffer(GL_BACK);

    /* opengl rendering done for now */
    gtk_gl_area_endgl(GTK_GL_AREA(glarea));
  }
  /* swap backbuffer to front */
  gtk_gl_area_swapbuffers(GTK_GL_AREA(glarea));

  return TRUE;
}


gint glarea_configure(GtkWidget *widget, GdkEventConfigure *event)
{
  /* ... OpenGL calls can be done only if begingl returns true ... */
  if (gtk_gl_area_begingl(GTK_GL_AREA(widget))) {
    glViewport(0,0, widget->allocation.width, widget->allocation.height);

    /* ... End opengl calls by calling endgl ... */
    gtk_gl_area_endgl(GTK_GL_AREA(widget));
  }
  return TRUE;
}


gint glarea_motion_notify(GtkWidget *widget, GdkEventMotion *event)
{
	gint			x, y;
	GdkRectangle		area;
	GdkModifierType		state;

  if (event->is_hint) {
    gdk_window_get_pointer(event->window, &x, &y, &state);
  } else {
    x = (gint)event->x;
    y = (gint)event->y;
    state = (GdkModifierType)event->state;
  }

  if(state & GDK_BUTTON1_MASK) {
    /* ... Drag in progress, simulate trackball ... */
    g_print("Mouse motion button 1 at coordinates (%d,%d)\n",x,y);

    /* ... Orientation has changed, redraw mesh ... */
    gtk_widget_draw(glarea, (GdkRectangle *)NULL);
  }

  if(state & GDK_BUTTON2_MASK) {
    /* ... Zooming drag ... */
    g_print("Mouse motion button 2 at coordinates (%d,%d)\n",x,y);

    /* ... Zoom has changed, redraw mesh ... */
    gtk_widget_draw(glarea, (GdkRectangle *)NULL);
  }

  if(state & GDK_BUTTON3_MASK) {
    /* ... 3rd button drag ... */
    g_print("Mouse motion button 3 at coordinates (%d,%d)\n",x,y);

    /* ... Zoom has changed, redraw mesh ... */
    gtk_widget_draw(glarea, (GdkRectangle *)NULL);
  }

  return TRUE;
}


gint glarea_button_press(GtkWidget *widget, GdkEventButton *event)
{
	gint	return_status=TRUE;

  switch(event->type) {
    case GDK_BUTTON_PRESS:
      g_print("Mouse button %d click at coordinates (%lf,%lf)\n",
              event->button,event->x,event->y);
      break;
    case GDK_2BUTTON_PRESS:
      g_print("Mouse button %d double-click at coordinates (%lf,%lf)\n",
              event->button,event->x,event->y);
      break;
    case GDK_3BUTTON_PRESS:
      g_print("Mouse button %d triple-click at coordinates (%lf,%lf)\n",
              event->button,event->x,event->y);
      break;
    default:
      g_print("Unknown button press event\n");
      return_status=FALSE;
      break;
  }

  return(return_status);
}


gint glarea_key_press_event(GtkWidget *widget, GdkEventKey *event)
{
  switch (event->keyval) {
    case GDK_r:
      g_print("Button r pressed...redrawing\n");
      gtk_widget_draw(glarea, (GdkRectangle *)NULL);
      break;
    case GDK_l:
      g_print("Button l pressed...redrawing\n");
      gtk_widget_draw(glarea, (GdkRectangle *)NULL);
      break;
    case GDK_p:
      g_print("Button p pressed...redrawing\n");
      gtk_widget_draw(glarea, (GdkRectangle *)NULL);
      break;
  }
  return (TRUE);
}


int main(int argc, char **argv)
{
	GtkWidget	*ctlwindow, *glwindow;

  /* ... Initialize gtk, handle command-line parameters ... */
  gtk_init(&argc, &argv);

  /* ... Check if OpenGL (GLX extension) is supported. ... */
  if(gdk_gl_query() == FALSE) {
    g_print("OpenGL not supported\n");
    return(0);
  }

  ctlwindow = create_ctl_window();
  if(!(glwindow = create_gl_window())) {
    g_print("Can't create GtkGlArea widget\n");
    return(0);
  }

  /* ... This is the event loop in gtk ... */
  /* ... Do not return until _gtk_main_quit_ is called ... */
  gtk_main();

  /* ... Exit status code ... */
  return(0);
}

GtkWidget *create_gl_window()
{
	GtkWidget	*glwindow;

  /* ... Create the window which will hold glarea ... */
  /* ... Create a window in gtk - note the window is NOT visible yet ... */
  glwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  /* ... Set window attributes ... */
  gtk_window_set_title(GTK_WINDOW(glwindow), "Scene");
  gtk_container_border_width(GTK_CONTAINER(glwindow), 5);
  /* create_popup_menu(glwindow);                 * add popup menu to window */
  gtk_signal_connect (GTK_OBJECT(glwindow), "delete_event",
                      GTK_SIGNAL_FUNC(gtk_main_quit), NULL);
  /* ... Destroy this window when exiting from gtk_main() ... */
  gtk_quit_add_destroy(1, GTK_OBJECT(glwindow));

  /* ... Create new OpenGL widget with GTK's window mode; analogous to
         glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  */
  glarea = gtk_gl_area_new_vargs(NULL,	      /* no sharing */
				 GDK_GL_DOUBLEBUFFER,
				 GDK_GL_RGBA,
				 GDK_GL_DEPTH_SIZE,1,
				 GDK_GL_NONE);/* last arg must be GDK_GL_NONE */
  if(!glarea) { 
    g_print("Can't create GtkGlArea widget\n");
    return FALSE;
  }

  /* ... Set up events and signals for OpenGL widget ... */
  gtk_widget_set_events(GTK_WIDGET(glarea),
                        GDK_EXPOSURE_MASK|
                        GDK_BUTTON_PRESS_MASK|
                        GDK_BUTTON_RELEASE_MASK|
                        GDK_KEY_PRESS_MASK|
                        GDK_KEY_RELEASE_MASK|
			GDK_POINTER_MOTION_MASK|
			GDK_POINTER_MOTION_HINT_MASK);
  gtk_signal_connect (GTK_OBJECT(glarea), "expose_event",
                      GTK_SIGNAL_FUNC(glarea_expose), NULL);
  gtk_signal_connect (GTK_OBJECT(glarea), "motion_notify_event",
                      GTK_SIGNAL_FUNC(glarea_motion_notify), NULL);
  gtk_signal_connect (GTK_OBJECT(glarea), "button_press_event",
                      GTK_SIGNAL_FUNC(glarea_button_press), NULL);
  gtk_signal_connect (GTK_OBJECT(glarea), "configure_event",
                      GTK_SIGNAL_FUNC(glarea_configure), NULL);
  gtk_signal_connect (GTK_OBJECT(glarea), "key_press_event",
                      GTK_SIGNAL_FUNC(glarea_key_press_event), NULL);
  gtk_signal_connect (GTK_OBJECT(glarea), "delete_event",
                      GTK_SIGNAL_FUNC(gtk_main_quit), NULL);
  gtk_widget_set_usize(glarea, 640, 480); /* minimum size */
  gtk_quit_add_destroy(1, GTK_OBJECT(glarea));

  /* ... Insert glarea into window and show it all ... */
  gtk_container_add(GTK_CONTAINER(glwindow),glarea);
  gtk_widget_show(glarea);
  gtk_widget_show(glwindow);

  /* ... Set focus to glarea widget and initialize OpenGL ... */
  GTK_WIDGET_SET_FLAGS(glarea,GTK_CAN_FOCUS);
  gtk_widget_grab_focus(GTK_WIDGET(glarea));
  initgl();

  return(glwindow);
}

GtkWidget *create_ctl_window()
{
	GtkWidget	*ctlwindow;

  /* ... Create the control window ... */
  /* ... Create a window in gtk - note the window is NOT visible yet ... */
  ctlwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  /* ... Call destroyapp when the window gets a "delete_event" signal ... */
  gtk_signal_connect(GTK_OBJECT(ctlwindow),"delete_event",
                     GTK_SIGNAL_FUNC(destroyapp), NULL);

  /* ... Put some breathing room around objects in the container ... */
  gtk_container_border_width(GTK_CONTAINER(ctlwindow),15);

  /* ... Add button to window ... */
  add_button(ctlwindow);

  /* ... Now, make the window visible ... */
  gtk_widget_show(ctlwindow);

  return(ctlwindow);
}


void add_button(GtkWidget *parent_window)
{
	GtkWidget	*button;

  /* ... Create a button ... */
  button = gtk_button_new_with_label("Click me");

  /* ... Give the button an event handler - it'll call the function */
  /* ... button_was_clicked when the button is clicked ... */
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
                     GTK_SIGNAL_FUNC(button_was_clicked),NULL);

  /* ... The main window contains the button ... */
  gtk_container_add(GTK_CONTAINER(parent_window),button);

  /* ... Make the button visible ... */
  gtk_widget_show(button);
}