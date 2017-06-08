/****************************************************************
 * e2.h								*		
 *								*
 * This program is in the public domain and you are using it at *
 * your own risk.						*
 *								*
 ****************************************************************/

#ifndef HEADER_E8
#define HEADER_E8
#include <algorithm>

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include <gtkmm/separator.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>

#include <vtkmm/renderingarea.h>

#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkImplicitFunction.h>
#include <vtkSampleFunction.h>
#include <vtkContourFilter.h>
#include <vtkImageData.h>
#include <vtkOutlineFilter.h>
#include <vtkCubeAxesActor2D.h>


class Function : public vtkImplicitFunction
{
	public:
	static Function* New();
		
	virtual double EvaluateFunction(double x[3]);		
	virtual void EvaluateGradient(double x[3], double g[3]);
	
	void setKoefficents(const double koeff[3])
	{ std::copy(koeff, koeff+3, koeff_); Modified(); }
	
	void getKoefficients(double koeff[3]) const
	{ std::copy(koeff_, koeff_+3, koeff); }

	void setPowers(const double powers[3])
	{ std::copy(powers, powers+3, pow_); Modified(); }
	
	void getPowers(double powers[3]) const
	{ std::copy(pow_, pow_+3, powers); }	
		
	protected:
	Function();		
	
	private:
	double koeff_[3];
	double pow_[3];	
};	


class MainWindow : public Gtk::Window
{
	public:
	MainWindow();
	virtual ~MainWindow();

	private:
	void cb_apply();
	void cb_radioToggled();
		
	void prepare();	
	void vtkInit();
	void configure();
	void build();
	void establishConnections();
	
	Gtk::HBox _hBoxMain;
	Gtk::Frame _frameConfig;
	Gtk::VBox _vBoxConfig;
	Gtk::Label _labelFunc;
	Gtk::Frame _frameFunc;
	Gtk::VBox _vBoxFunc;
	Gtk::Label _labelExpanation;
	Gtk::HSeparator _hSep;
	Gtk::HBox _hBoxFunc;
	Gtk::VBox _vBoxKoeff;
	Gtk::HBox _hBoxKx, _hBoxKy, _hBoxKz;
	Gtk::Label _labelKx, _labelKy, _labelKz;
	Gtk::SpinButton _spinKx, _spinKy, _spinKz;
	Gtk::VSeparator _vSep;
	Gtk::VBox _vBoxPow;
	Gtk::HBox _hBoxPx, _hBoxPy, _hBoxPz;
	Gtk::Label _labelPx, _labelPy, _labelPz;
	Gtk::SpinButton _spinPx, _spinPy, _spinPz;
	Gtk::Label _labelContour;
	Gtk::Frame _frameContour;
	Gtk::VBox _vBoxContour;
	Gtk::Frame _frameRange;
	Gtk::HBox _hBoxRange;
	Gtk::Label _labelMin, _labelMax;
	Gtk::SpinButton _spinMin, _spinMax;
	Gtk::Frame _frameValues;
	Gtk::HBox _hBoxValues;
	Gtk::SpinButton _spinValues;
	Gtk::Label _labelDisplay;
	Gtk::Frame _frameDisplay;
	Gtk::HBox _hBoxDisplay;
	Gtk::RadioButton _radioOutlines, _radioAxis;
	Gtk::HButtonBox _hbBox;
	Gtk::Button _bQuit, _bApply;
	Gtk::Frame _frameVtk;
	
	Gtk::Vtk::RenderingArea _vtkArea;	
	
	vtkSampleFunction *_p_sampleFunc;
	vtkContourFilter *_p_contourFilter;
	vtkPolyDataMapper *_p_contourMapper;
	vtkActor *_p_contourActor;
	Function *_p_implicitFunc;
	
	vtkOutlineFilter *_p_outlineFilter;
	vtkPolyDataMapper *_p_outlineMapper;
	vtkActor *_p_outlineActor;
	
	vtkCubeAxesActor2D *_p_cubeAxesActor2D;
	
	double _a_k[3], _a_p[3];
	double _a_range[2];
	int _values;
	
	static const double _sc_kx=1, _sc_ky=1, _sc_kz=1;
	static const double _sc_px=2, _sc_py=2, _sc_pz=2;
	static const double _sc_min=0, _sc_max=1;
	static const int _sc_values=5;	
};

#endif
