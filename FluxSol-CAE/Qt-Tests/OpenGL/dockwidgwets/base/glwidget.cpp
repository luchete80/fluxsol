#include "glwidget.h"
#include <QtOpenGL/QGLWidget>

GLWidget::GLWidget()
	: QGLWidget()
{
	setAutoBufferSwap(false);
}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL ()
{
	glClearColor(191.0f/255.0f, 191.0f/255.0f, 191.0f/255.0f, 1.0f);
}

void GLWidget::paintGL()
{
	makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	swapBuffers();
}