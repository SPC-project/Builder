#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <GL/glu.h>
#include <QGLWidget>
#include <QTimer>
#include <QApplication>
#include <QtGui/QMouseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <qdebug.h>

#include <QMainWindow>
#include <fstream>

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QSlider;
class GLWidget;
class QPushButton;

class myGLWidget : public QGLWidget
{
	Q_OBJECT
public:
	explicit myGLWidget(QWidget *parent = 0);

public:
	void      initializeGL(); // функция инициализации OpenGL
	void      resizeGL(int w, int h); //функция обработки изменения размера вывода
	void      paintGL(); // функция прорисовки


	void      drawAxis();//прорисовка осей
	void      drawFigure();//прорисовка фигуры
	void      drawGrid();//прорисовка сетки
	void      drawPoints();


	void      zoom(bool plus);
	void      rotate_x(bool clockwise, GLfloat angle = 0.15);//повернуть фигуру вокруг x
	void      rotate_y(bool clockwise, GLfloat angle = 0.15);//повернуть фигуру вокруг у
	void      rotate_z(bool clockwise, GLfloat angle = 0.15);//повернуть вокруг z
	void      move_x(bool left);//переиестить камеру по х
	void      move_y(bool up);//переместить камеру по у
	void      see_x(bool left);//переместить обозреваемую точку по х
	void      see_y(bool up);//переместить обозреваемую точку по у
	void      move_only_cam_x(bool left);
	void      move_only_cam_y(bool up);

	bool      drawgr;//флаги отрисовки сетки, фигуры и осей
	bool      drawfig;
	bool      drawax;
	bool      drawdot;
	bool      move_cam;
	bool      changeNC;
	double    minPotent;//минимальное значение напряженности
	double    maxPotent;//максимальное значение напряженности
	double    gradColorStep;//шаг напряженность соответствующий 1 тону цвета


	public slots:
	void      setStandartLook();//вернуться в стандартную позицию
	void      getProection(int side);//повернуть фигуру одной из проекций 1-6
	int       readFile(char * from);


private:
	void      drawElement(int f1, int f2, int f3, int f4, int b1, int b2, int b3, int b4); //прорисовка 1го элемента
	void      drawGridElement(int f1, int f2, int f3, int f4, int b1, int b2, int b3, int b4);//прорисовка 1 ячейки сетки
	void      drawPointElement(int f1, int f2, int f3, int f4, int b1, int b2, int b3, int b4);// Прорисовка одной точки
	void      createArea();//инициализируем массивы
	void      mouseMoveEvent(QMouseEvent*me);
	void      setColor(double value);//выбор цвета узла
	double    setColorStep();//задание шага соответствия напряженность-цвет

	GLfloat   x_angle;//значения поворота фигуры относительно осей
	GLfloat   y_angle;
	GLfloat   z_angle;
	GLfloat   seeAtX;//координаты обозреваемой точки
	GLfloat   seeAtY;
	GLfloat   amAtX;//координаты расположения камеры
	GLfloat   amAtY;

	GLfloat   camY; //верх камеры Y
	GLfloat   camX; //верх камеры X


	GLfloat   factorZoom;//множитель для масштабирования
	int       nElements; // количество элементов
	int       nInds;//количество узлов

	double    mouse_x;//координаты мышки 
	double    mouse_y;


	int**     inds;//матрица элементов
	GLfloat** koor;//матрица координат
	double*   potent;//вектор напряжений в узлах

protected:
	void      wheelEvent(QWheelEvent* pe);
};

#endif // MYGLWIDGET_H
