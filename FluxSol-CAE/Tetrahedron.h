#ifndef _TETRA_H_
#define _TETRA_H_

#include <QApplication>
#include <QGLWidget>
#include <QtOpenGL/QGLWidget>
#include <QMouseEvent>
#include <QColorDialog>
#include <iostream>
#include <GL/glu.h>
#include <QtOpenGL/QGLFormat>

class Tetrahedron : public QGLWidget
{
	Q_OBJECT
	public:
	Tetrahedron(QWidget *parent = 0);
	protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	private:
	void draw();
	int faceAtPosition(const QPoint &pos);
	GLfloat rotationX;
	GLfloat rotationY;
	GLfloat rotationZ;
	QColor faceColors[4];
	QPoint lastPos;
};
#endif