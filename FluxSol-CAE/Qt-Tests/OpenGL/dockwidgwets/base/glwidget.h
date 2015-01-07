#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL/QGLWidget>

class GLWidget : public QGLWidget {
public:
	GLWidget();
	~GLWidget();

protected:
	void initializeGL();
	void paintGL();
};

#endif
