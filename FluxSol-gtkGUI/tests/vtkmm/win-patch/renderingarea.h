/* vtkmm - for using vtk with gtkmm
 * Copyright (C) 2005 Matthias Langer <mlangc@gmx.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef H_VTKMM_RENDERING_AREA
#define H_VTKMM_RENDERING_AREA
#include <gtkmm/drawingarea.h>
#include <gdkmm.h>
#include <gtkmm/widget.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <gtk/gtkgl.h>
namespace Gtk {
namespace Vtk {

/*!@brief Use this one for rendering via vtk into your gtkmm application.
 * @remark 
 *	Don't use vtkInteractor in connection with this window, as this doesn't work
 * 	well with gtk. Use set_interactive(bool) instead.*/	
class RenderingArea : public DrawingArea
{
	public:
	RenderingArea() : interactive_(true), buttonsMask_(0), lastX_(0), lastY_(0)
	{ gl_configure(); }

	virtual ~RenderingArea();	
	
    /*!@brief Returns the underlying vtkRenderWindow.
     * */
	vtkRenderWindow* get_vtk_window() { return p_vtkWindow_; }

    /*!@brief Returns the underlying vtkRenderer.
     * */
	vtkRenderer* get_vtk_renderer() { return p_renderer_; }
	
    /*!@brief Controls wether the window is interactive.*/
	void set_interactive(bool setting=true) { interactive_ = setting; buttonsMask_ = 0; }

    /*!@brief Returns true iff the window is interactive.*/
	bool get_interactive() const { return interactive_; }
	
    /*!@brief Returns the width of the RenderingArea.*/
	virtual int get_rendering_area_width() const { return get_width(); }

    /*!@brief Returns the height of the RenderingArea.*/
	virtual int get_rendering_area_height() const  { return get_height(); }
	
	protected:
	virtual void on_realize();
	virtual bool on_expose_event(GdkEventExpose*);
	virtual bool on_button_press_event(GdkEventButton *p_event);
	virtual bool on_button_release_event(GdkEventButton *p_event);
	virtual bool on_motion_notify_event(GdkEventMotion *p_event);
	virtual bool on_scroll_event(GdkEventScroll *p_event);
	virtual bool on_configure_event(GdkEventConfigure* p_configure);

	private:
	//no copy, no assignment
	RenderingArea(const RenderingArea&);
	RenderingArea& operator=(const RenderingArea&);
	void gl_configure();
	
	//for user interaction
	bool interactive_;
	int buttonsMask_;
	double lastX_, lastY_;	
	
	//vtk objects
	vtkRenderWindow *p_vtkWindow_;
	vtkRenderer *p_renderer_;	
};	
	
	
}}//namespaces
#endif //_H_VTKMM_