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

#define G_OS_WIN32
#include "renderingarea.h"
#include <iostream>
#ifdef G_OS_WIN32
#include "gdk/gdkwin32.h"
#else
#include <gdk/gdkx.h>
#endif
#include <vtkCamera.h>
#include <stdexcept>
namespace Gtk {
namespace Vtk {

RenderingArea::~RenderingArea()
{ p_renderer_->Delete(); p_vtkWindow_->Delete(); }

void RenderingArea::on_realize()
{
	DrawingArea::on_realize();
#ifdef G_OS_WIN32
	void *p_windowId = reinterpret_cast<void*>(GDK_WINDOW_HWND(get_window()->gobj()));
#else
	void *p_windowId = reinterpret_cast<void*>(GDK_WINDOW_XWINDOW(get_window()->gobj()));
#endif
	p_vtkWindow_->SetWindowId(p_windowId);
    p_vtkWindow_->SetSize(get_rendering_area_width(), get_rendering_area_height());
}

bool RenderingArea::on_expose_event(GdkEventExpose*)
{ p_vtkWindow_->Render(); return true; }

bool RenderingArea::on_button_press_event(GdkEventButton *p_event)
{
	if(interactive_)
	{
		buttonsMask_ |= (1 << p_event->button);
		lastX_ = p_event->x;
		lastY_ = p_event->y;
	}
	return true;
}

bool RenderingArea::on_button_release_event(GdkEventButton *p_event)
{
	if(interactive_)
		buttonsMask_ &= ~(1 << p_event->button);
	return true;
}

bool RenderingArea::on_motion_notify_event(GdkEventMotion *p_event)
{
	if(!interactive_)
		return true;

	double dx = p_event->x - lastX_;
	double dy = p_event->y - lastY_;
	lastX_ = p_event->x;
	lastY_ = p_event->y;

	bool b1Pressed = (buttonsMask_ == (1 << 1));
	bool b2Pressed = (buttonsMask_ == (1 << 2));
	bool b3Pressed = (buttonsMask_ == (1 << 3));

	vtkCamera *p_camera = p_renderer_->GetActiveCamera();
	if(b1Pressed)
	{
		p_camera->Azimuth(-1.0 * dx);
		p_camera->Elevation(1.0 * dy);
		p_camera->OrthogonalizeViewUp();
		p_renderer_->ResetCameraClippingRange();
		queue_draw();
	}
	else if(b2Pressed)
	{
		p_camera->Roll(1.0*dx);
		p_camera->Zoom(exp(0.01*dy));
		queue_draw();
	}
	else if(b3Pressed)
	{
		double dl = dx / get_rendering_area_width();
		double dh = dy / get_rendering_area_height();

		double dir[3];
		p_camera->GetDirectionOfProjection(dir);
		double up[3];
		p_camera->GetViewUp(up);
		double pos[3];
		p_camera->GetPosition(pos);
		double foc[3];
		p_camera->GetFocalPoint(foc);

		double ds[3];
		ds[0] = dh * up[0] + dl * (up[1] * dir[2] - up[2] * dir[1]);
    	ds[1] = dh * up[1] + dl * (up[2] * dir[0] - up[0] * dir[2]);
   	 	ds[2] = dh * up[2] + dl * (up[0] * dir[1] - up[1] * dir[0]);

		foc[0] += ds[0];
    	foc[1] += ds[1];
    	foc[2] += ds[2];
    	pos[0] += ds[0];
    	pos[1] += ds[1];
    	pos[2] += ds[2];

		p_camera->SetPosition(pos);
		p_camera->SetFocalPoint(foc);
		p_renderer_->ResetCameraClippingRange();
		queue_draw();
	}
	return true;
}

bool RenderingArea::on_scroll_event(GdkEventScroll *p_event)
{
	if(p_event->direction == GDK_SCROLL_DOWN)
	{
		p_renderer_->GetActiveCamera()->Zoom(exp(-0.2));
		queue_draw();
	}
	else if(p_event->direction == GDK_SCROLL_UP)
	{
		p_renderer_->GetActiveCamera()->Zoom(exp(0.2));
		queue_draw();
	}
	return true;
}

bool RenderingArea::on_configure_event(GdkEventConfigure *p_event)
{
	p_vtkWindow_->SetSize(p_event->width, p_event->height);
	return true;
}

void RenderingArea::gl_configure()
{

	int glMode =  GDK_GL_MODE_RGBA | GDK_GL_MODE_DOUBLE;
	glMode |= GDK_GL_MODE_DEPTH;

	GdkGLConfig *pc_p_glConfig =
	gdk_gl_config_new_by_mode(static_cast<GdkGLConfigMode>(glMode));

	if(pc_p_glConfig != NULL)
	{
		bool tmp = gtk_widget_set_gl_capability
		(GTK_WIDGET(this->gobj()), pc_p_glConfig, NULL, TRUE, GDK_GL_RGBA_TYPE);
		if(!tmp)
		{
			std::cerr << "gtk_widget_set_gl_capability(...) failed !" << std::endl;
			exit(1);
		}
	}
	else
	{
		std::cerr << "gdk_gl_config_new(...) failed !" << std::endl;
		exit(1);
	}

	Gdk::EventMask events = Gdk::EXPOSURE_MASK | Gdk::BUTTON_PRESS_MASK;
	events |= Gdk::BUTTON_RELEASE_MASK  | Gdk::BUTTON_MOTION_MASK;
	events |= Gdk::SCROLL_MASK;
	this->set_events(events);

	p_vtkWindow_ = vtkRenderWindow::New();
 	p_renderer_ = vtkRenderer::New();
  	p_vtkWindow_->AddRenderer(p_renderer_);
}
}}//namespaces
