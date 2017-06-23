
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL);
VTK_MODULE_INIT(vtkRenderingContextOpenGL);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);

// OSG Includes
#include <OSGGLUT.h>
#include <OSGConfig.h>
#include <OSGSimpleGeometry.h>
#include <OSGGLUTWindow.h>
#include <OSGSimpleSceneManager.h>
#include <OSGPerspectiveCamera.h>

// VTK includes
#include <ExternalVTKWidget.h>
#include <vtkActor.h>
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include <vtkCubeSource.h>
#include <vtkExternalOpenGLRenderWindow.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

// defines
#define FOV 75.f
#define NEAR_PLANE 0.01f
#define FAR_PLANE 100.0f

// variables
vtkNew<ExternalVTKWidget> externalVTKWidget;
vtkNew<vtkActor> actor;
int width = 100, height = 100;
OSG::SimpleSceneManagerRefPtr mgr;
OSG::GLUTWindowRefPtr gwin;

using namespace OSG;

// redraw the window
void display(void)
{
	// activate OSG window
	gwin->activate();
	gwin->frameInit();

	// draw OSG stuff
	mgr->update();
	gwin->renderAllViewports(mgr->getRenderAction());

	// setup gl stuff for camera and lights (why?!?)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (FOV, (float)width/(float)height, NEAR_PLANE, FAR_PLANE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// draw VTK stuff
	actor->RotateX(1.0f);
	actor->RotateY(1.0f);
	externalVTKWidget->GetRenderWindow()->Render();

	// swap buffers and deactivate OSG window
	gwin->swap();
	gwin->frameExit();
	gwin->deactivate();
}

// react to size changes
void reshape(int w, int h)
{
	width = w;
	height = h;
	mgr->resize(w, h);
	glutPostRedisplay();
}

// react to mouse button presses
void mouse(int button, int state, int x, int y)
{
	if (state)
		mgr->mouseButtonRelease(button, x, y);
	else
		mgr->mouseButtonPress(button, x, y);
	glutPostRedisplay();
}

// react to mouse motions with pressed buttons
void motion(int x, int y)
{
	mgr->mouseMove(x, y);
	glutPostRedisplay();
}

// react to keys
void keyboard(unsigned char k, int x, int y)
{
	switch(k)
	{
	case 27:
		{
			mgr = NULL;
			gwin = NULL;
			OSG::osgExit();
			exit(0);
		}
		break;
	}
}


int main(int argc, char **argv)
{
	// init and setup GLUT
	glewInit();
	OSG::osgInit(argc,argv);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	int winid = glutCreateWindow("OpenSG");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

	{
		// Connection between GLUT and OpenSG
		gwin = OSG::GLUTWindow::create();
		gwin->setGlutId(winid);
		gwin->setSize(100, 100);
		gwin->init();

		// create the scene
		OSG::NodeRefPtr scene = OSG::makeTorus(.1f, 0.6f, 16, 16);
		OSG::commitChanges();
		mgr = OSG::SimpleSceneManager::create();
		mgr->setWindow(gwin );
		mgr->setRoot  (scene);
		mgr->showAll();

		// set the camera parameters
		CameraRecPtr cam = mgr->getCamera();
		PerspectiveCamera *pPerspCam = OSG::dynamic_pointer_cast<PerspectiveCamera>(cam);
		pPerspCam->setFov(FOV);
		pPerspCam->setNear(NEAR_PLANE);
		pPerspCam->setFar(FAR_PLANE);
		pPerspCam->setAspect(1.f);
	}

	{
		// create the VTK scene
		vtkNew<vtkCubeSource> cs;
		vtkNew<vtkPolyDataMapper> mapper;
		mapper->SetInputConnection(cs->GetOutputPort());

		actor->SetMapper(mapper.GetPointer());
		actor->GetProperty()->SetColor(1,1,0.2);
		actor->GetProperty()->SetOpacity(0.5);

		// create the VTK external renderer
		vtkNew<vtkExternalOpenGLRenderWindow> renWin;
		externalVTKWidget->SetRenderWindow(renWin.GetPointer());
		vtkRenderer* ren = externalVTKWidget->AddRenderer();
		ren->AddActor(actor.GetPointer());
	}

	glutMainLoop();
	return 0;
}
