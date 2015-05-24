#include "MyGlWidget.h"
#include <cmath>
#include <fstream>

void myGLWidget::initializeGL()
{

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);//цвет фона
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);//загружаем матрицы проекции
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);//и пмодели-просмотра


}
void myGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(amAtX, amAtY, -10.f, seeAtX, seeAtY, 0.0f, 0.0f, 1.0f, 0.0f);//устанавливаем место расположение камеры и обозреваемую точку
	glRotatef(x_angle, 0.0f, 1.0f, 0.0f);//устанавливаем наклон фигуры относительно осей
	glRotatef(y_angle, 1.0f, 0.0f, 0.0f);
	glRotatef(z_angle, 0.0f, 0.0f, 1.0f);

	glScalef(factorZoom*(-1), factorZoom, factorZoom);//масштабируем фигуру


	if (drawfig)
		drawFigure();//рисуем фигуру
	if (drawgr)
		drawGrid();//сетку
	if (drawax)
	{
		drawPoints();
		drawAxis();//оси
	}


}

void myGLWidget::resizeGL(int w, int h)
{
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}


myGLWidget::myGLWidget(QWidget *parent) :
QGLWidget(parent)
{
	factorZoom = 0.5f;
	x_angle = y_angle = z_angle = 0.0f;
	amAtX = seeAtX = seeAtY = 0.0f;
	amAtY = 5.0f;
	drawgr = 1;
	drawfig = 1;
	drawax = 0;//по умолчанию оси не рисуем
}



void myGLWidget::mouseMoveEvent(QMouseEvent * me)
{
	/*Проверка сильно большого отклонения - например, чтобы сцена не
	самовращалась при сворачивании-розварачивании окна, при
	перемещении на большое расстояние курсора с отпускнием и т.д.
	Не есть обязательным - но так естественнее*/

	if (abs(me->x() - mouse_x) >= 20)
		mouse_x = me->x();
	if (abs(me->y() - mouse_y) >= 20)
		mouse_y = me->y();
	//Меняем угол поворота в зависимости от положения мыши
	y_angle += (mouse_y - me->y()) / 2;
	x_angle -= (mouse_x - me->x()) / 2;
	//Сохраняем текущее значение координат курсора для возможности определения
	//скорости их изменения.
	mouse_x = me->x();
	mouse_y = me->y();

	
	//Вызываем метод, который обработает поворот и выведет обновленное изображение

	updateGL();
}

void myGLWidget::drawElement(int f1, int f2, int f3, int f4, int b1, int b2, int b3, int b4){

	//выбираем значение цвета для данного узла   и рисуем его
	setColor(potent[f1]);		glVertex3f(koor[f1][0], koor[f1][1], koor[f1][2]);
	setColor(potent[f2]);		glVertex3f(koor[f2][0], koor[f2][1], koor[f2][2]);
	setColor(potent[f3]);		glVertex3f(koor[f3][0], koor[f3][1], koor[f3][2]);
	setColor(potent[f4]);		glVertex3f(koor[f4][0], koor[f4][1], koor[f4][2]);

	setColor(potent[f1]);		glVertex3f(koor[f1][0], koor[f1][1], koor[f1][2]);
	setColor(potent[b1]);		glVertex3f(koor[b1][0], koor[b1][1], koor[b1][2]);
	setColor(potent[b2]);		glVertex3f(koor[b2][0], koor[b2][1], koor[b2][2]);
	setColor(potent[f2]);		glVertex3f(koor[f2][0], koor[f2][1], koor[f2][2]);

	setColor(potent[f1]);		glVertex3f(koor[f1][0], koor[f1][1], koor[f1][2]);
	setColor(potent[f4]);		glVertex3f(koor[f4][0], koor[f4][1], koor[f4][2]);
	setColor(potent[b4]);		glVertex3f(koor[b4][0], koor[b4][1], koor[b4][2]);
	setColor(potent[b1]);		glVertex3f(koor[b1][0], koor[b1][1], koor[b1][2]);

	setColor(potent[f3]);		glVertex3f(koor[f3][0], koor[f3][1], koor[f3][2]);
	setColor(potent[b3]);		glVertex3f(koor[b3][0], koor[b3][1], koor[b3][2]);
	setColor(potent[b4]);		glVertex3f(koor[b4][0], koor[b4][1], koor[b4][2]);
	setColor(potent[f4]);		glVertex3f(koor[f4][0], koor[f4][1], koor[f4][2]);

	setColor(potent[f2]);		glVertex3f(koor[f2][0], koor[f2][1], koor[f2][2]);
	setColor(potent[b2]);		glVertex3f(koor[b2][0], koor[b2][1], koor[b2][2]);
	setColor(potent[b3]);		glVertex3f(koor[b3][0], koor[b3][1], koor[b3][2]);
	setColor(potent[f3]);		glVertex3f(koor[f3][0], koor[f3][1], koor[f3][2]);

	setColor(potent[b1]);		glVertex3f(koor[b1][0], koor[b1][1], koor[b1][2]);
	setColor(potent[b2]);		glVertex3f(koor[b2][0], koor[b2][1], koor[b2][2]);
	setColor(potent[b3]);		glVertex3f(koor[b3][0], koor[b3][1], koor[b3][2]);
	setColor(potent[b4]);		glVertex3f(koor[b4][0], koor[b4][1], koor[b4][2]);
}

void  myGLWidget::wheelEvent(QWheelEvent* pe){

	//В зависимости от направления движения колесика увеличиваем/уменьшаем фигуру
	if ((pe->delta())>0)
		factorZoom *= 1.1f;
	else if ((pe->delta())<0)
		factorZoom /= 1.1f;

	updateGL();//перерисовываем сцену

}

void  myGLWidget::createArea(){
	//создаем массивы
	inds = new int*[nElements];
	for (int i = 0; i < nElements; i++)
		inds[i] = new int[8];


	koor = new GLfloat*[nInds];
	potent = new double[nInds];
	for (int i = 0; i < nInds; i++){
		koor[i] = new GLfloat[3];
		potent[i] = 0;//обнуляем массив напряжений на случай, если напряжения не заданы
	}


}

int myGLWidget::readFile(char * from){
    std::ifstream input(from);
    if( ! input.is_open() ){
        QMessageBox readError;
        QString desc(tr("Ошибка во время чтения файла: "));
        readError.setText(desc+from);
        readError.exec();
        return 1;
    }

	input >> nElements >> nInds;//считываем количество элементов и узлов

	createArea();//создаем массивы

	//считываем данные
	for (int i = 0; i < nElements; i++)
		for (int j = 0; j < 8; j++)
			input >> inds[i][j];

	for (int i = 0; i < nInds; i++)
		for (int j = 0; j < 3; j++)
			input >> koor[i][j];

	for (int i = 0; i < nInds; i++)
		input >> potent[i];


	gradColorStep = setColorStep();//выбираем градацию цветов
    return 0;
}

void myGLWidget::drawAxis()
{

	glLineWidth(3.0f);
	// до вызова команды ширина равна 9 пикселю по умолчанию

	// устанавливаем цвет последующих примитивов
	glColor4f(1.00f, 0.00f, 0.00f, 1.0f);
	// ось x красного цвета
	glBegin(GL_LINES); // построение линии
	glVertex3f(9.0f, 0.0f, 0.0f); // первая точка
	glVertex3f(-9.0f, 0.0f, 0.0f); // вторая точка
	glEnd();

	QColor halfGreen(0, 128, 0, 255);
	qglColor(halfGreen);
	glBegin(GL_LINES);
	// ось y зеленого цвета
	glVertex3f(0.0f, 9.0f, 0.0f);
	glVertex3f(0.0f, -9.0f, 0.0f);

	glColor4f(0.00f, 0.00f, 1.00f, 9.0f);
	// ось z синего цвета
	glVertex3f(0.0f, 0.0f, 9.0f);
	glVertex3f(0.0f, 0.0f, -9.0f);
	glEnd();
	/*
		перемещение локальных поординат 
		над
		сделать.
		пздц
	*/

}

void myGLWidget::drawFigure(){
	//рисуем четырехугольники
	glBegin(GL_QUADS);
	for (int i = nElements - 1; i >= 0; i--)
	{//поочередно отрисовываем все элементы
		drawElement(inds[i][0] - 1, inds[i][1] - 1, inds[i][2] - 1, inds[i][3] - 1, inds[i][4] - 1,
			inds[i][5] - 1, inds[i][6] - 1, inds[i][7] - 1);
		
		
	}
	//отнимаем единицу, т.к.  нумерация с 1, а не с 0
	glEnd();
	
}

void myGLWidget::drawPoints(){
	for (int i = nElements - 1; i >= 0; i--)
	{
		drawPointElement(inds[i][0] - 1, inds[i][1] - 1, inds[i][2] - 1, inds[i][3] - 1,
			inds[i][4] - 1, inds[i][5] - 1, inds[i][6] - 1, inds[i][7] - 1);

	}
}

void myGLWidget::drawPointElement(int f1, int f2, int f3, int f4, int b1, int b2, int b3, int b4){
	//рисуем сетку
	glPointSize(8);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	glColor3d(255, 0, 200);
	glVertex3f(koor[f4][0], koor[f4][1], koor[f4][2]);
	glVertex3f(koor[f1][0], koor[f1][1], koor[f1][2]);
	glVertex3f(koor[f2][0], koor[f2][1], koor[f2][2]);
	glVertex3f(koor[f3][0], koor[f3][1], koor[f3][2]);
	glVertex3f(koor[f4][0], koor[f4][1], koor[f4][2]);

	glVertex3f(koor[b4][0], koor[b4][1], koor[b4][2]);
	glVertex3f(koor[b1][0], koor[b1][1], koor[b1][2]);
	glVertex3f(koor[b2][0], koor[b2][1], koor[b2][2]);
	glVertex3f(koor[b3][0], koor[b3][1], koor[b3][2]);

	glVertex3f(koor[f3][0], koor[f3][1], koor[f3][2]);
	glVertex3f(koor[f2][0], koor[f2][1], koor[f2][2]);
	glVertex3f(koor[b2][0], koor[b2][1], koor[b2][2]);
	glVertex3f(koor[b1][0], koor[b1][1], koor[b1][2]);
	glVertex3f(koor[f1][0], koor[f1][1], koor[f1][2]);

	

	glEnd();


}


void myGLWidget::drawGrid(){
	//толщина 2 пикселя, черный цвет
	glLineWidth(2.0f);
	glColor3f(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < nElements; i++)
		drawGridElement(inds[i][0] - 1, inds[i][1] - 1, inds[i][2] - 1, inds[i][3] - 1,
		inds[i][4] - 1, inds[i][5] - 1, inds[i][6] - 1, inds[i][7] - 1);

}

void myGLWidget::drawGridElement(int f1, int f2, int f3, int f4, int b1, int b2, int b3, int b4){
	glBegin(GL_LINE_STRIP);//выбираем замкнутые линии

	//рисуем сетку
	glVertex3f(koor[f4][0], koor[f4][1], koor[f4][2]);
	glVertex3f(koor[f1][0], koor[f1][1], koor[f1][2]);
	glVertex3f(koor[f2][0], koor[f2][1], koor[f2][2]);
	glVertex3f(koor[f3][0], koor[f3][1], koor[f3][2]);
	glVertex3f(koor[f4][0], koor[f4][1], koor[f4][2]);

	glVertex3f(koor[b4][0], koor[b4][1], koor[b4][2]);
	glVertex3f(koor[b1][0], koor[b1][1], koor[b1][2]);
	glVertex3f(koor[b2][0], koor[b2][1], koor[b2][2]);
	glVertex3f(koor[b3][0], koor[b3][1], koor[b3][2]);

	glVertex3f(koor[f3][0], koor[f3][1], koor[f3][2]);
	glVertex3f(koor[f2][0], koor[f2][1], koor[f2][2]);
	glVertex3f(koor[b2][0], koor[b2][1], koor[b2][2]);
	glVertex3f(koor[b1][0], koor[b1][1], koor[b1][2]);
	glVertex3f(koor[f1][0], koor[f1][1], koor[f1][2]);

	glEnd();
}

void myGLWidget::setColor(double value){
	int c1, c2, c3;//c1 - start color, c2 - middle color, c3 - finish  color
	int state = (int)((value - minPotent) / gradColorStep);//находим цвет соответствующий данному значению

	if (state <= 255){
		c1 = 255;//от чисто синего 
		c2 = state;
		c3 = 0;
	}
	else if (state > 255 && state <= 512){
		c1 = 512 - state;
		c2 = 255;
		c3 = 0;
	}//до зеленого
	else if (state > 512 && state < 768){
		c1 = 0;//от зеленого
		c2 = 255;
		c3 = state;
	}
	else{
		c1 = 0;
		c2 = 1024 - state;
		c3 = 255;//до красного
	}

	glColor3ub(c3, c2, c1);//задаем выбранный цвет
}

double myGLWidget::setColorStep(){
	double min, max;
	min = max = potent[0];

	for (int i = 0; i < nInds; i++){
		if (potent[i] > max)
			max = potent[i];
		if (potent[i] < min)
			min = potent[i];
	}//находим минимальное и максимальное значение

	minPotent = min;
	maxPotent = max;

	return (max - min) / 1024;//возвращаем шаг напряженностей одного тона цвета
}

void myGLWidget::zoom(bool plus){
	if (plus)
		factorZoom *= 1.1;
	else
		factorZoom /= 1.1;
	updateGL();
}

void myGLWidget::rotate_x(bool clockwise, GLfloat angle){
	if (clockwise)
		x_angle += angle;
	else
		x_angle -= angle;
	updateGL();
}

void myGLWidget::rotate_y(bool clockwise, GLfloat angle){
	if (clockwise)
		y_angle += angle;
	else
		y_angle -= angle;
	updateGL();
}

void myGLWidget::rotate_z(bool clockwise, GLfloat angle){
	if (clockwise)
		z_angle += angle;
	else
		z_angle -= angle;
	updateGL();
}

void myGLWidget::see_x(bool left){
	if (left)
		seeAtX -= 0.1f;
	else
		seeAtX += 0.1f;
	updateGL();
}

void myGLWidget::see_y(bool up){
	if (up)
		seeAtY += 0.1f;
	else
		seeAtY -= 0.1f;
	updateGL();

}

void myGLWidget::move_only_cam_x(bool left){
	if (left){
		gluLookAt(seeAtX+=0.1f, seeAtY, 0, amAtX, amAtY, 0,
			camX += 0.1f, 1.0, 0.0);
	}
	else{
		gluLookAt(seeAtX-=0.1f, seeAtY, 0, amAtX, amAtY, 0,
			camX -= 0.1f, 1.0, 0.0);
	}
	updateGL();
}

void myGLWidget::move_x(bool left){
	if (left)
		amAtX += 0.1f;
	else{
		amAtX -= 0.1f;
	}
	see_x(left);
	updateGL();
}

void myGLWidget::move_y(bool up){
	if (up)
		amAtY += 0.1f;
	else
		amAtY -= 0.1f;
	see_y(up);
	updateGL();
}



void myGLWidget::setStandartLook(){
	amAtX = 0.0f;//задаем стандартную позицию камеры и фигуры
	amAtY = 5.0f;
	seeAtX = 0.0f;
	seeAtY = 0.0f;
	getProection(1);

	updateGL();

}

void myGLWidget::getProection(int side){

	switch (side){
	case 1:{
		z_angle = 0;
		y_angle = 0;
		x_angle = 0;
		break; }
	case 2: {
		z_angle = 0;
		y_angle = 0;
		x_angle = 90;
		break; }
	case 3: {
		z_angle = 0;
		y_angle = 0;
		x_angle = 180;
		break; }
	case 4: {
		z_angle = 0;
		y_angle = 0;
		x_angle = 270;
		break; }
	case 5: {
		z_angle = 0;
		y_angle = 90;
		x_angle = 0;
		break; }
	case 6:	{
		z_angle = 0;
		y_angle = 270;
		x_angle = 0;
		break; }
	}

	updateGL();
}

