#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <GL/glu.h>
#include <QGLWidget>
#include <QTimer>
#include <QApplication>
#include <QtGui/QMouseEvent>
#include <QMessageBox>
#include <QFileDialog>
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
	void      initializeGL(); // ������� ������������� OpenGL
	void      resizeGL(int w, int h); //������� ��������� ��������� ������� ������
	void      paintGL(); // ������� ����������


	void      drawAxis();//���������� ����
	void      drawFigure();//���������� ������
	void      drawGrid();//���������� �����


	void      zoom(bool plus);
	void      rotate_x(bool clockwise, GLfloat angle = 0.15);//��������� ������ ������ x
	void      rotate_y(bool clockwise, GLfloat angle = 0.15);//��������� ������ ������ �
	void      rotate_z(bool clockwise, GLfloat angle = 0.15);//��������� ������ z
	void      move_x(bool left);//����������� ������ �� �
	void      move_y(bool up);//����������� ������ �� �
	void      see_x(bool left);//����������� ������������ ����� �� �
	void      see_y(bool up);//����������� ������������ ����� �� �
	void      move_only_cam_x(bool left);
	void      move_only_cam_y(bool up);

	bool      drawgr;//����� ��������� �����, ������ � ����
	bool      drawfig;
	bool      drawax;
	bool      move_cam;
	bool      changeNC;
	double    minPotent;//����������� �������� �������������
	double    maxPotent;//������������ �������� �������������
	double    gradColorStep;//��� ������������� ��������������� 1 ���� �����


	public slots:
	void      setStandartLook();//��������� � ����������� �������
	void      getProection(int side);//��������� ������ ����� �� �������� 1-6
	void      readFile(char * from);


private:
	void      drawElement(int f1, int f2, int f3, int f4, int b1, int b2, int b3, int b4); //���������� 1�� ��������
	void      drawGridElement(int f1, int f2, int f3, int f4, int b1, int b2, int b3, int b4);//���������� 1 ������ �����
	void      createArea();//�������������� �������
	void      mouseMoveEvent(QMouseEvent*me);
	void      setColor(double value);//����� ����� ����
	double    setColorStep();//������� ���� ������������ �������������-����

	GLfloat   x_angle;//�������� �������� ������ ������������ ����
	GLfloat   y_angle;
	GLfloat   z_angle;
	GLfloat   seeAtX;//���������� ������������ �����
	GLfloat   seeAtY;
	GLfloat   amAtX;//���������� ������������ ������
	GLfloat   amAtY;

	GLfloat   camY; //���� ������ Y
	GLfloat   camX; //���� ������ X


	GLfloat   factorZoom;//��������� ��� ���������������
	int       nElements; // ���������� ���������
	int       nInds;//���������� �����

	double    mouse_x;//���������� ����� 
	double    mouse_y;


	int**     inds;//������� ���������
	GLfloat** koor;//������� ���������
	double*   potent;//������ ���������� � �����

protected:
	void      wheelEvent(QWheelEvent* pe);
};

#endif // MYGLWIDGET_H
