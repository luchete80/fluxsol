/****************************************************************
 * e1.cc							*		 *								*		 *								*
 * This program is in the public domain and you are using it at *
 * your own risk.						*
 *								*
 ****************************************************************/
#include <gtkmm/main.h>
#include <glibmm/random.h>
#include <glibmm/main.h>

#include "e1.h"
#include <limits>
#include <stdexcept>
#include <vtkCamera.h>
#include <gtkmm/stock.h>

using namespace Gtk;
using namespace Glib;

using std::numeric_limits;
namespace { typedef MainWindow MW; }
const int MW::_sc_coneRes, MW::_sc_phiRes, MW::_sc_thetaRes;
const double MW::_sc_lightR, MW::_sc_lightG, MW::_sc_lightB;
const double MW::_sc_spikeR, MW::_sc_spikeG, MW::_sc_spikeB;
const double MW::_sc_scaleFac;

namespace {
inline gushort percentageToGushort(double val)
{
	if(fabs(val) > 1)
		throw std::invalid_argument("percentageToGuishort(...)");

	const gushort max = numeric_limits<gushort>::max();
	return static_cast<gushort>(val*max);
}

inline double gushortToPercentage(gushort val)
{
	const gushort max = numeric_limits<gushort>::max();
	return static_cast<double>(val) / static_cast<double>(max);
}

inline void gdkColorFromVtkColor(Gdk::Color& col, double r, double g, double b)
{
	const gushort gr = percentageToGushort(r);
	const gushort gg = percentageToGushort(g);
	const gushort gb = percentageToGushort(b);
	col.set_rgb(gr, gg, gb);
}

inline void vtkColorFromGdkColor(double& r, double& g, double& b, const Gdk::Color& col)
{
	r = col.get_red_p();
	g = col.get_green_p();
	b = col.get_blue_p();
}
}//namespace

MainWindow::MainWindow()
: _labelSpikes("Spikes"), _labelLight("Light"),
  _frameCone("Cone resolution"), _frameFac("Scale factor"),
  _frameTheta("Theta resolution"),
  _bStop(Stock::STOP), _bAnimate("Animate")
{
	configure();
	build();
	vtkInit();
}

MainWindow::~MainWindow()
{
	_p_sphereSource->Delete();
	_p_elevationFilter->Delete();
	_p_dataSetMapper->Delete();
	_p_sphereActor->Delete();
	_p_coneSource->Delete();
	_p_glyph->Delete();
	_p_spikeDataMapper->Delete();
	_p_spikeActor->Delete();
	_p_light->Delete();
}

void MainWindow::configure()
{
	set_title("vtkmm demo");
	set_size_request(700, 600);
	set_border_width(5);

	_vBoxMain.set_spacing(5);
	_hBoxColors.set_border_width(3);
	_hBoxSpikes.set_spacing(15);
	_hBoxLight.set_spacing(15);
	_vBoxParams.set_border_width(3);
	_vBoxParams.set_spacing(3);
	_vBoxCone.set_border_width(5);
	_vBoxFac.set_border_width(5);
	_vBoxTheta.set_border_width(5);


	_labelColors.set_markup("<b>Colors</b>");
	_labelParams.set_markup("<b>Parameters</b>");

	_frameColors.set_label_widget(_labelColors);
	_frameParams.set_label_widget(_labelParams);

	_frameColors.set_label_align(0.5);
	_frameParams.set_label_align(0.5);

	Gdk::Color gdkCol;
	gdkColorFromVtkColor(gdkCol, _sc_lightR, _sc_lightG, _sc_lightB);
	_cButLight.set_color(gdkCol);
	gdkColorFromVtkColor(gdkCol, _sc_spikeR, _sc_spikeG, _sc_spikeB);
	_cButSpikes.set_color(gdkCol);

	_hScaleCone.set_value_pos(POS_BOTTOM);
	_hScaleCone.set_range(3, 50);
	_hScaleCone.set_digits(0);
	_hScaleCone.set_value(_sc_coneRes);

	_hScaleTheta.set_value_pos(POS_BOTTOM);
	_hScaleTheta.set_range(3, 50);
	_hScaleTheta.set_digits(0);
	_hScaleTheta.set_value(_sc_thetaRes);

	_hScaleFac.set_value_pos(POS_BOTTOM);
	_hScaleFac.set_range(0.1, 2);
	_hScaleFac.set_digits(2);
	_hScaleFac.set_value(_sc_scaleFac);

	_bStop.set_sensitive(false);

	establishConnections();
}

void MainWindow::build()
{
	add(_vBoxMain);
		_vBoxMain.pack_start(_frameVtk, PACK_EXPAND_WIDGET);
			_frameVtk.add(_vtkArea);
		_vBoxMain.pack_start(_frameColors, PACK_SHRINK);
			_frameColors.add(_hBoxColors);
				_hBoxColors.pack_start(_vBoxSpikes, PACK_SHRINK);
					_vBoxSpikes.pack_start(_hBoxSpikes, PACK_SHRINK);
						_hBoxSpikes.pack_start(_labelSpikes, PACK_SHRINK);
						_hBoxSpikes.pack_start(_cButSpikes, PACK_SHRINK);
				_hBoxColors.add(_vSeperator);
				_hBoxColors.pack_start(_vBoxLight, PACK_SHRINK);
					_vBoxLight.pack_start(_hBoxLight, PACK_SHRINK);
						_hBoxLight.pack_start(_labelLight, PACK_SHRINK);
						_hBoxLight.pack_start(_cButLight, PACK_SHRINK);
		_vBoxMain.pack_start(_frameParams, PACK_SHRINK);
			_frameParams.add(_vBoxParams);
				_vBoxParams.add(_frameCone);
					_frameCone.add(_vBoxCone);
						_vBoxCone.add(_hScaleCone);
				_vBoxParams.add(_frameTheta);
					_frameTheta.add(_vBoxTheta);
						_vBoxTheta.add(_hScaleTheta);
				_vBoxParams.add(_frameFac);
					_frameFac.add(_vBoxFac);
						_vBoxFac.add(_hScaleFac);
		_vBoxMain.pack_start(_hbBox, PACK_SHRINK);
			_hbBox.add(_bStop);
			_hbBox.add(_bAnimate);
	show_all_children();
}

void MainWindow::vtkInit()
{
	_p_sphereSource = vtkSphereSource::New();
		_p_sphereSource->SetPhiResolution(_sc_phiRes);
		_p_sphereSource->SetThetaResolution(_sc_thetaRes);

	_p_elevationFilter = vtkElevationFilter::New();
		//_p_elevationFilter->SetInput(_p_sphereSource->GetOutput());
		_p_elevationFilter->SetInputConnection(_p_sphereSource->GetOutputPort());
		_p_elevationFilter->SetLowPoint(0, 0, -1);
		_p_elevationFilter->SetHighPoint(0, 0, 1);

	_p_dataSetMapper = vtkDataSetMapper::New();
		//_p_dataSetMapper->SetInput(_p_elevationFilter->GetOutput());
		_p_dataSetMapper->SetInputConnection(_p_elevationFilter->GetOutputPort());

	_p_sphereActor = vtkActor::New();
		_p_sphereActor->SetMapper(_p_dataSetMapper);

	_p_coneSource = vtkConeSource::New();
		_p_coneSource->SetResolution(_sc_coneRes);

	_p_glyph = vtkGlyph3D::New();
//		_p_glyph->SetInput(_p_sphereSource->GetOutput());
		//_p_glyph->SetSource(_p_coneSource->GetOutput());
//		_p_glyph->SetVectorModeToUseNormal();
//		_p_glyph->SetScaleModeToScaleByVector();
//		_p_glyph->SetScaleFactor(_sc_scaleFac);

	_p_spikeDataMapper = vtkPolyDataMapper::New();
		//_p_spikeDataMapper->SetInput(_p_glyph->GetOutput());
		_p_spikeDataMapper->SetInputConnection(_p_glyph->GetOutputPort());

	_p_spikeActor = vtkActor::New();
		_p_spikeActor->SetMapper(_p_spikeDataMapper);
		_p_spikeActor->GetProperty()->SetColor(_sc_spikeR,_sc_spikeG, _sc_spikeB);

	_p_light = vtkLight::New();
		_p_light->SetColor(_sc_lightR, _sc_lightG, _sc_lightB);
		_p_light->SetLightTypeToHeadlight();

	vtkRenderer *p_renderer = _vtkArea.get_vtk_renderer();
		p_renderer->AddActor(_p_spikeActor);
		p_renderer->AddActor(_p_sphereActor);
		p_renderer->AddLight(_p_light);

p_renderer->AddLight(_p_light);

}

void MainWindow::establishConnections()
{
	using sigc::mem_fun;

	_cButLight.signal_color_set().connect
	(mem_fun(*this, &MainWindow::cb_lightColor));

	_cButSpikes.signal_color_set().connect
	(mem_fun(*this, &MainWindow::cb_spikeColor));

	_hScaleCone.signal_value_changed().connect
	(mem_fun(*this, &MainWindow::cb_coneRes));

	_hScaleTheta.signal_value_changed().connect
	(mem_fun(*this, &MainWindow::cb_thetaRes));

	_hScaleFac.signal_value_changed().connect
	(mem_fun(*this, &MainWindow::cb_scaleFac));

	_bStop.signal_clicked().connect
	(mem_fun(*this, &MainWindow::cb_stop));

	_bAnimate.signal_clicked().connect
	(mem_fun(*this, &MainWindow::cb_animate));

	Glib::signal_timeout().connect
	(mem_fun(*this, &MainWindow::cb_timeout), 20);
	//Glib::Rand rand(1234);
}

void MainWindow::cb_lightColor()
{
	Gdk::Color gdkCol(_cButLight.get_color());
	double r, g, b;
	vtkColorFromGdkColor(r, g, b, gdkCol);
	_p_light->SetColor(r, g, b);
	_vtkArea.get_vtk_window()->Render();
}

void MainWindow::cb_spikeColor()
{
	Gdk::Color gdkCol(_cButSpikes.get_color());
	double r, g, b;
	vtkColorFromGdkColor(r, g, b, gdkCol);
	_p_spikeActor->GetProperty()->SetColor(r, g, b);
	_vtkArea.get_vtk_window()->Render();
}

void MainWindow::cb_coneRes()
{
	int res = static_cast<int>(_hScaleCone.get_value());
	_p_coneSource->SetResolution(res);
	_vtkArea.get_vtk_window()->Render();
}

void MainWindow::cb_thetaRes()
{
	int res = static_cast<int>(_hScaleTheta.get_value());
	_p_sphereSource->SetThetaResolution(res);
	_vtkArea.get_vtk_window()->Render();
}

void MainWindow::cb_scaleFac()
{
	double fac = _hScaleFac.get_value();
	_p_glyph->SetScaleFactor(fac);
	_vtkArea.get_vtk_window()->Render();
}

void MainWindow::cb_stop()
{
	_bStop.set_sensitive(false);
	_bAnimate.set_sensitive(true);
}

void MainWindow::cb_animate()
{
	_bStop.set_sensitive(true);
	_bAnimate.set_sensitive(false);
}

bool MainWindow::cb_timeout()
{
	if(_bStop.is_sensitive())
	{
		vtkCamera *p_cam = _vtkArea.get_vtk_renderer()->GetActiveCamera();
		p_cam->Azimuth(1);
		_vtkArea.get_vtk_renderer()->ResetCameraClippingRange();
		//p_cam->Elevation(1);
		p_cam->OrthogonalizeViewUp();
		_vtkArea.get_vtk_window()->Render();
	}
	return true;
}

int main(int argc, char **argv)
{
	Main kit(argc, argv);
	Vtk::init(argc, argv);
	MainWindow win;
	Main::run(win);
}
