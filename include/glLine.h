#ifndef GLLINE_H
#define GLLINE_H
#include "MyGlWidget.h"

class glLine : public QGLWidget
{
	Q_OBJECT
public:
	explicit glLine(QWidget *parent = 0);

public:
	void     initializeGL(); // ������� ������������� OpenGL
	void     resizeGL(int w, int h); //������� ��������� ��������� ������� ������
	void     paintGL(); // ������� ����������
};

#endif // GLLINE_H
