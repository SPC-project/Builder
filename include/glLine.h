#ifndef GLLINE_H
#define GLLINE_H
#include "MyGlWidget.h"

class glLine : public QGLWidget
{
	Q_OBJECT
public:
	explicit glLine(QWidget *parent = 0);

public:
	void     initializeGL(); // функция инициализации OpenGL
	void     resizeGL(int w, int h); //функция обработки изменения размера вывода
	void     paintGL(); // функция прорисовки
};

#endif // GLLINE_H
