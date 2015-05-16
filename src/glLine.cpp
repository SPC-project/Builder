#include "glLine.h"

glLine::glLine(QWidget *parent) : QGLWidget(parent){
	update();
}

void glLine::initializeGL()
{

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);


}
void glLine::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glScalef(-4.0f, 4.0f, 1.0f);
	gluLookAt(0.0f, 0.0f, -10.f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	glBegin(GL_QUAD_STRIP);

	glColor3ub(0, 0, 255);
	glVertex2f(-20, -5);
	glVertex2f(-20, 5);

	glColor3ub(0, 255, 0);
	glVertex2f(0, -5);
	glVertex2f(0, 5);

	glColor3ub(255, 0, 0);
	glVertex2f(20, -5);
	glVertex2f(20, 5);


	glEnd();


}

void glLine::resizeGL(int w, int h)
{
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}
