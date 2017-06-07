/****************************************************************
 * e1.h								*
 *								*
 * This program is in the public domain and you are using it at *
 * your own risk.						*
 *								*
 ****************************************************************/

#ifndef HEADER_E7
#define HEADER_E7

#include <vtkmm.h>

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkGlyph3D.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkElevationFilter.h>
#include <vtkDataSetMapper.h>
#include <vtkLight.h>

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/scale.h>
#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/button.h>
#include <gtkmm/colorbutton.h>
#include <gtkmm/separator.h>

class MainWindow : public Gtk::Window
{
	public:
	MainWindow();
	virtual ~MainWindow();

	private:
	void configure();
	void build();
	void vtkInit();
	void establishConnections();

	//callbacks
	void cb_lightColor();
	void cb_spikeColor();
	void cb_coneRes();
	void cb_thetaRes();
	void cb_scaleFac();
	void cb_stop();
	void cb_animate();
	bool cb_timeout();
	///////////

	//////////////////////////////////
	//for gtkmm
	Gtk::VBox _vBoxMain;
	Gtk::Frame _frameVtk;
	Gtk::Vtk::RenderingArea _vtkArea;

	Gtk::Label _labelColors;
	Gtk::Frame _frameColors;
	Gtk::HBox _hBoxColors;
	Gtk::HBox _hBoxSpikes;
	Gtk::VSeparator _vSeperator;
	Gtk::VBox _vBoxSpikes;
	Gtk::Label _labelSpikes;
	Gtk::ColorButton _cButSpikes;
	Gtk::VBox _vBoxLight;
	Gtk::HBox _hBoxLight;
	Gtk::Label _labelLight;
	Gtk::ColorButton _cButLight;

	Gtk::Label _labelParams;
	Gtk::Frame _frameParams;
	Gtk::VBox _vBoxParams;
	Gtk::Frame _frameCone;
	Gtk::VBox _vBoxCone;
	Gtk::HScale _hScaleCone;
	Gtk::Frame _frameFac;
	Gtk::VBox _vBoxFac;
	Gtk::HScale _hScaleFac;
	Gtk::Frame _frameTheta;
	Gtk::VBox _vBoxTheta;
	Gtk::HScale _hScaleTheta;

	Gtk::HButtonBox _hbBox;
	Gtk::Button _bStop, _bAnimate;
	/////////////////////////////////
	//for vtk
	vtkActor *_p_coneActor;
	vtkConeSource *_p_coneSource;
	vtkPolyDataMapper *_p_coneDataMapper;
	/////////////////////////////////
	//static constant data
	static const int _sc_coneRes = 6, _sc_phiRes = 8, _sc_thetaRes = 8;
	static const double _sc_lightR = 1.0, _sc_lightG = 1.0, _sc_lightB = 1.0;
	static const double _sc_spikeR = 1.0, _sc_spikeG = 0.0, _sc_spikeB = 0.0;
	static const double _sc_scaleFac = 0.25;

};

#endif
