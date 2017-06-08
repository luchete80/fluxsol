/****************************************************************
 * e2.cc							*		
 *								*
 * This program is in the public domain and you are using it at *
 * your own risk.						*
 *								*
 ****************************************************************/


#include "e2.h"
#include <vtkmm.h>
#include <gtkmm/main.h>
#include <gtkmm/stock.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
using namespace Gtk; using Glib::ustring;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////// static constants ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef MainWindow MW;
const double MW::_sc_kx, MW::_sc_ky, MW::_sc_kz;
const double MW::_sc_px, MW::_sc_py, MW::_sc_pz;
const double MW::_sc_min, MW::_sc_max;
const int MW::_sc_values;

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Function /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
Function::Function()
{
	for(unsigned i=0; i != 3; ++i)
	{
		koeff_[i] = 1;
		pow_[i] = 2;
	}
}

Function* Function::New()
{
	Function *p_ret = new Function;
	p_ret->Register(p_ret);
	return p_ret;
}

double Function::EvaluateFunction(double x[3])
{
	double ret = 0;
	for(unsigned i=0; i != 3; ++i)
		ret += koeff_[i]*pow(x[i], pow_[i]);
	return ret;
}

void Function::EvaluateGradient(double x[3], double g[3])
{
	for(unsigned i=0; i !=3; ++i)
		g[i] = koeff_[i]*pow_[i]*pow(x[i], pow_[i]-1);
}	

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// MainWindow ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow()
: _labelKx("kx: "), _labelKy("ky: "), _labelKz("kz: "),
  _labelPx("px: "), _labelPy("py: "), _labelPz("pz: "),
  _frameRange("Range"), 
  _labelMin("min: "), _labelMax("max: "),
  _frameValues("Values"),
  _radioOutlines("Outlines"), _radioAxis("Axis"),
  _bQuit(Stock::QUIT), _bApply(Stock::APPLY), 
  _values(_sc_values)
{
	prepare();
	vtkInit();
	configure();
	build();
}

MainWindow::~MainWindow()
{
	_p_sampleFunc->Delete();
	_p_contourFilter->Delete();
	_p_contourMapper->Delete();
	_p_contourActor->Delete();
	_p_implicitFunc->Delete();
	_p_outlineFilter->Delete();
	_p_outlineMapper->Delete();
	_p_outlineActor->Delete();
	_p_cubeAxesActor2D->Delete();
}

void MainWindow::cb_apply()
{
	_a_k[0] = _spinKx.get_value();
	_a_k[1] = _spinKy.get_value();
	_a_k[2] = _spinKz.get_value();
	
	_a_p[0] = _spinPx.get_value();
	_a_p[1] = _spinPy.get_value();
	_a_p[2] = _spinPz.get_value();
	
	_a_range[0] = _spinMin.get_value();
	_a_range[1] = _spinMax.get_value();
	
	_values = static_cast<int>(_spinValues.get_value());
	
	_p_implicitFunc->setKoefficents(_a_k);
	_p_implicitFunc->setPowers(_a_p);
	_p_contourFilter->GenerateValues(_values, _a_range);
	
	_vtkArea.get_vtk_window()->Render();
}

void MainWindow::cb_radioToggled()
{
	bool outlines = _radioOutlines.get_active();
	vtkRenderer *p_renderer = _vtkArea.get_vtk_renderer();
	if(outlines)
	{
		p_renderer->RemoveActor(_p_cubeAxesActor2D);
		p_renderer->AddActor(_p_outlineActor);
	}
	else
	{
		p_renderer->RemoveActor(_p_outlineActor);
		p_renderer->AddActor(_p_cubeAxesActor2D);
	}
	_vtkArea.get_vtk_window()->Render();
}
		

void MainWindow::prepare()
{
	_a_k[0] = _sc_kx;
	_a_k[1] = _sc_ky;
	_a_k[2] = _sc_kz;
	
	_a_p[0] = _sc_px;
	_a_p[1] = _sc_py;
	_a_p[2] = _sc_pz;
	
	_a_range[0] = _sc_min;
	_a_range[1] = _sc_max;
}

void MainWindow::vtkInit()
{
	_p_implicitFunc = Function::New();
		_p_implicitFunc->setKoefficents(_a_k);
		_p_implicitFunc->setPowers(_a_p);
	
	vtkRenderer *p_renderer = _vtkArea.get_vtk_renderer();
	p_renderer->SetBackground(0.5,0.5,0.5);
	
	_p_sampleFunc = vtkSampleFunction::New();
		_p_sampleFunc->SetImplicitFunction(_p_implicitFunc);
		_p_sampleFunc->SetSampleDimensions(30, 30, 30);
		_p_contourFilter = vtkContourFilter::New();
			_p_contourFilter->SetInput(_p_sampleFunc->GetOutput());
			_p_contourFilter->GenerateValues(_values, _a_range);
			_p_contourMapper = vtkPolyDataMapper::New();
				_p_contourMapper->SetInput(_p_contourFilter->GetOutput());
					_p_contourActor = vtkActor::New();
						_p_contourActor->SetMapper(_p_contourMapper);
							p_renderer->AddActor(_p_contourActor);
	
	_p_outlineFilter = vtkOutlineFilter::New();	
		_p_outlineFilter->SetInput(_p_sampleFunc->GetOutput());
		_p_outlineMapper = vtkPolyDataMapper::New();
			_p_outlineMapper->SetInput(_p_outlineFilter->GetOutput());
			_p_outlineActor = vtkActor::New();
				_p_outlineActor->SetMapper(_p_outlineMapper);
				_p_outlineActor->GetProperty()->SetColor(0, 0, 0);
	
	_p_cubeAxesActor2D = vtkCubeAxesActor2D::New();	
		_p_cubeAxesActor2D->SetInput(_p_outlineFilter->GetOutput());
		_p_cubeAxesActor2D->SetCamera(p_renderer->GetActiveCamera());
		_p_cubeAxesActor2D->SetFlyModeToOuterEdges();
		
	p_renderer->AddActor(_p_cubeAxesActor2D);
	p_renderer->ResetCamera();
}

void MainWindow::configure()
{
	set_title("e8");
	set_size_request(640, 480);
	set_border_width(5);
	
	_hBoxMain.set_spacing(5);
	
	_vBoxConfig.set_border_width(3);
	_vBoxConfig.set_spacing(10);
	
	_vBoxFunc.set_border_width(3);
	_hBoxFunc.set_spacing(3);
	
	_vBoxKoeff.set_spacing(7);
	_vBoxPow.set_spacing(7);
	
	_hBoxKx.set_spacing(2);
	_hBoxKy.set_spacing(2);
	_hBoxKz.set_spacing(2);
	_hBoxPx.set_spacing(2);
	_hBoxPy.set_spacing(2);
	_hBoxPz.set_spacing(2);
	
	_vBoxContour.set_border_width(3);
	_vBoxContour.set_spacing(3);
	_hBoxRange.set_spacing(2);

	_labelExpanation.set_text("kx*x^px + ky*y^py + kz*z^kz");

	_labelFunc.set_markup("<b>Function</b>");
	_labelContour.set_markup("<b>Contour</b>");
	_labelDisplay.set_markup("<b>Display</b>");
	_frameFunc.set_label_widget(_labelFunc);
	_frameContour.set_label_widget(_labelContour);
	_frameDisplay.set_label_widget(_labelDisplay);
	_frameFunc.set_label_align(0.5);
	_frameContour.set_label_align(0.5);
	_frameDisplay.set_label_align(0.5);
	
	_spinKx.set_range(-10,10);
	_spinKy.set_range(-10,10);
	_spinKz.set_range(-10,10);
	
	_spinPx.set_range(-10,10);
	_spinPy.set_range(-10,10);
	_spinPz.set_range(-10,10);
	
	_spinKx.set_digits(1);
	_spinKy.set_digits(1);
	_spinKz.set_digits(1);
	
	_spinKx.set_increments(0.1, 0.5);
	_spinKy.set_increments(0.1, 0.5);
	_spinKz.set_increments(0.1, 0.5);
	
	_spinPx.set_increments(1, 2);
	_spinPy.set_increments(1, 2);
	_spinPz.set_increments(1, 2);
	
	_spinKx.set_value(_sc_kx);
	_spinKy.set_value(_sc_ky);
	_spinKz.set_value(_sc_kz);
	
	_spinPx.set_value(_sc_px);
	_spinPy.set_value(_sc_py);
	_spinPz.set_value(_sc_pz);
	
	_spinMin.set_range(-10,10);
	_spinMax.set_range(-10,10);
	
	_spinMin.set_digits(1);
	_spinMax.set_digits(1);
	
	_spinMin.set_value(_sc_min);
	_spinMax.set_value(_sc_max);
	
	_spinMin.set_increments(0.1, 0.5);
	_spinMax.set_increments(0.1, 0.5);
		
	_spinValues.set_range(1,20);
	_spinValues.set_increments(1, 2);
	_spinValues.set_value(_sc_values);
	
	_hBoxDisplay.set_border_width(5);
	_hBoxDisplay.set_spacing(3);
		
	RadioButton::Group radioGroup(_radioOutlines.get_group());
	_radioAxis.set_group(radioGroup);
	_radioAxis.set_active();
	
	establishConnections();
}

void MainWindow::build()
{
	add(_hBoxMain);
		_hBoxMain.pack_start(_frameConfig, PACK_SHRINK);
			_frameConfig.add(_vBoxConfig);
				_vBoxConfig.pack_start(_frameDisplay, PACK_SHRINK);
					_frameDisplay.add(_hBoxDisplay);
						_hBoxDisplay.add(_radioOutlines);
						_hBoxDisplay.pack_end(_radioAxis, PACK_SHRINK);		
				_vBoxConfig.add(_frameFunc);
					_frameFunc.add(_vBoxFunc);
						_vBoxFunc.add(_labelExpanation);
							_vBoxFunc.add(_hSep);
							_vBoxFunc.pack_start(_hBoxFunc, PACK_EXPAND_WIDGET);
								_hBoxFunc.add(_vBoxKoeff);
									_vBoxKoeff.add(_hBoxKx);
										_hBoxKx.add(_labelKx);
										_hBoxKx.add(_spinKx);
									_vBoxKoeff.add(_hBoxKy);
										_hBoxKy.add(_labelKy);
										_hBoxKy.add(_spinKy);
									_vBoxKoeff.add(_hBoxKz);
										_hBoxKz.add(_labelKz);
										_hBoxKz.add(_spinKz);
								_hBoxFunc.add(_vSep);
								_hBoxFunc.add(_vBoxPow);
									_vBoxPow.add(_hBoxPx);
										_hBoxPx.add(_labelPx);
										_hBoxPx.add(_spinPx);
									_vBoxPow.add(_hBoxPy);
										_hBoxPy.add(_labelPy);
										_hBoxPy.add(_spinPy);
									_vBoxPow.add(_hBoxPz);
										_hBoxPz.add(_labelPz);
										_hBoxPz.add(_spinPz);
				_vBoxConfig.add(_frameContour);
					_frameContour.add(_vBoxContour);
						_vBoxContour.add(_frameRange);
							_frameRange.add(_hBoxRange);
								_hBoxRange.add(_labelMin);
								_hBoxRange.add(_spinMin);
								_hBoxRange.add(_labelMax);
								_hBoxRange.add(_spinMax);
						_vBoxContour.add(_frameValues);
							_frameValues.add(_hBoxValues);
								_hBoxValues.pack_start(_spinValues, PACK_EXPAND_PADDING);
				_vBoxConfig.pack_end(_hbBox, PACK_SHRINK);
					_hbBox.add(_bQuit);
					_hbBox.add(_bApply);
		_hBoxMain.add(_frameVtk);
			_frameVtk.add(_vtkArea);
	show_all_children();			
}

void MainWindow::establishConnections()
{
	using sigc::mem_fun;
	
	_bApply.signal_clicked().connect
	(mem_fun(*this, &MainWindow::cb_apply));
	
	_bQuit.signal_clicked().connect
	(mem_fun(*this, &MainWindow::hide));
	
	_radioOutlines.signal_toggled().connect
	(mem_fun(*this, &MainWindow::cb_radioToggled));

}	
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	Gtk::Main kit(argc, argv);
	Vtk::init(argc, argv);
	MainWindow win;
	Gtk::Main::run(win);
}
