#include <cmath>
#include "MyGlWidget.h"

myGLWidget::myGLWidget(QWidget *parent) : QGLWidget(parent)
{
	factorZoom = 0.5;
	x_angle = y_angle = z_angle = 0.0;
	amAtX = seeAtX = seeAtY = 0.0;
	amAtY = 5.0;
    camX = 0.0;
	drawgr = true;
	drawfig = true;
	drawax = false;//по умолчанию оси не рисуем
    drawair = true;
    nInds = 0;
    nElements = 0;
}

void myGLWidget::initializeGL()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);//цвет фона
	glClearDepth(1.0f);
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
    materials = new int[nInds];
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
		input >> materials[i];

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
    if( drawair )
        for (int i = nElements - 1; i >= 0; i--)
            drawElement(inds[i][0] - 1, inds[i][1] - 1, inds[i][2] - 1, inds[i][3] - 1, inds[i][4] - 1,
                inds[i][5] - 1, inds[i][6] - 1, inds[i][7] - 1); //отнимаем единицу, ибо нумерация с 1, а не с 0
    else { // Не отрисовываем "воздушные элементы"
        bool isAirElement;
        int edge1, edge2, edge3, edge4, edge5, edge6, edge7, edge8;
        for (int i = nElements - 1; i >= 0; i--) {
            edge1 = inds[i][0] - 1;
            edge2 = inds[i][1] - 1;
            edge3 = inds[i][2] - 1;
            edge4 = inds[i][3] - 1;
            edge5 = inds[i][4] - 1;
            edge6 = inds[i][5] - 1;
            edge7 = inds[i][6] - 1;
            edge8 = inds[i][7] - 1;
            isAirElement = materials[ edge1 ] == 0 || materials[ edge2 ] == 0 
                        || materials[ edge3 ] == 0 || materials[ edge4 ] == 0
                        || materials[ edge5 ] == 0 || materials[ edge6 ] == 0
                        || materials[ edge7 ] == 0 || materials[ edge8 ] == 0;
            if( isAirElement )
                continue;
            drawElement(edge1, edge2, edge3, edge4, edge5, edge6, edge7, edge8 );
        }
    }
	// TODO Нужно как-то намечать грани фигуры в случае, если отключена сетка (drawgrid) - иначе изображение с трудом идентифицируеться
	glEnd();
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

// TODO Текущий метод отрисовки - безумие! Смежные лини прорисовываются дважды, невидимые слои и ребра все равно рисуются
void myGLWidget::drawGrid(){
	//толщина 2 пикселя, черный цвет
	glLineWidth(2.0f);
    bool isGridOfAirElement;

	for (int i = 0; i < nElements; i++){
        isGridOfAirElement = false;
        for (int vertex = 0; vertex < 8; vertex++) {
            if( materials[ inds[i][vertex] - 1 ] == 0 ){
                isGridOfAirElement = true;
                break;
            }
        }

        if( isGridOfAirElement ) {
            if( drawair )
                if( drawfig )
                    glColor3f(1.0f, 1.0f, 1.0f); // белый
                else
                    glColor3f(1.0f, 1.0f, 0.0f); // желтый (чтобы было видно на желтом фоне)
            else
                continue;
        } else
            glColor3f(0.0f, 0.0f, 0.0f);
		drawGridElement(inds[i][0] - 1, inds[i][1] - 1, inds[i][2] - 1, inds[i][3] - 1,
            inds[i][4] - 1, inds[i][5] - 1, inds[i][6] - 1, inds[i][7] - 1);
    }
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

// Удаление элемента - реализация Даниила Шестопалова
//--------------------------------------------------------------------------------------------------
void  myGLWidget::deleteElement(int n){ //хотим удалить элемент под номером f1
	if ((temp_nElements > 0) && (n-1 < temp_nElements)&&(n > 0))
		for (int j = 0; j < 8; j++)
			temp_inds[n-1][j] = -1; // Просто заменяем значение его точек на -1. Удаление произойдет в функции remesh.

}

void  myGLWidget::deleteArea() { //пока что без входных параметров, так как непонятно, что именно мы удаляем
	tempArea();//создаем временную рабочую область
		
	//Тут должна находится рабочая часть функции. Перечисление элементов что удаляем.

	for (int i = 300; i < 400; i++)
		deleteElement(i);



	// Основной принцип заключается в том, что мы заменяем удаленные элементы во временном массиве индексов на -1 значения. Далее сортируем, смотрим что осталось и обновляем основные массивы

	remesh();//в этой функции происходит обновление основного массива индексов. Удаление выпавших узлов, переиндексация.
	update_main(); // обновляем основные массивы, удаляем временные. Тесно связана с функцией remesh. 
	paintGL();//перерисовываем фигуру

}

void  myGLWidget::tempArea() {

	//создаем временные (temp) массивы.
	create_temp();

	//Заполняем

	for (int i = 0; i < temp_nElements; i++)
		for (int j = 0; j < 8; j++)
		{
		temp_inds[i][j] = inds[i][j];
		}

	for (int i = 0; i < temp_nInds; i++)
		temp_potent[i] = potent[i];

	for (int i = 0; i < nInds; i++)
		for (int j = 0; j < 3; j++)
			temp_koor[i][j] = koor[i][j];


}

void  myGLWidget::update_main() {


	//очищаем старый массив элементов
	for (int i = 0; i < nElements; i++)
		delete inds[i];
	delete inds;

	for (int i = 0; i < nInds; i++)
		delete koor[i];
	delete koor;

	delete potent;


	//создаем массивs
	inds = new int*[temp_nElements];
	for (int i = 0; i < temp_nElements; i++)
		inds[i] = new int[8];

	koor = new GLfloat*[temp_nInds];
	potent = new double[temp_nInds];

	for (int i = 0; i < temp_nInds; i++){
		koor[i] = new GLfloat[3];
		potent[i] = 0;
	}

	//записываем в них данные из temp. заполнение основного массива на основе новой индексации. основан на связи старой и новой индексации
	for (int i = 0; i < temp_nElements; i++)
		for (int j = 0; j < 8; j++)
			inds[i][j] = temp_A[temp_inds[i][j] - 1] + 1;
		
	for (int i = 0; i < temp_nInds; i++)
		potent[i] = temp_potent[temp_B[i]];

	for (int i = 0; i < temp_nInds; i++)
		for (int j = 0; j < 3; j++)
			koor[i][j] = temp_koor[temp_B[i]][j];

	//обновляем число элементов, узлов
	nElements = temp_nElements;
	nInds = temp_nInds;

	//удаляем temp массивы
	delete_temp();


}
void  myGLWidget::create_temp(){

	temp_nElements = nElements;
	temp_nInds = nInds;

	temp_inds = new int*[temp_nElements];
	for (int i = 0; i < temp_nElements; i++)
		temp_inds[i] = new int[8];


	temp_koor = new GLfloat*[nInds];
	temp_potent = new double[nInds];
	for (int i = 0; i < nInds; i++){
		temp_koor[i] = new GLfloat[3];
		temp_potent[i] = 0;//обнуляем массив напряжений на случай, если напряжения не заданы
	}

	temp_A = new int[temp_nInds];
	temp_B = new int[temp_nInds];

}

void  myGLWidget::delete_temp() {

	for (int i = 0; i < nElements; i++)
		delete[] temp_inds[i];
	delete[] temp_inds;

	for (int i = 0; i < nInds; i++)
		delete temp_koor[i];
	delete temp_koor;

	delete temp_potent;

	delete temp_A;
	delete temp_B;
}

void  myGLWidget::remesh(){

	for (int i = 0; i < nInds; i++){ // два важных массива индексов.
		temp_A[i] = i; // Индекс А содержит нетронутую индексацию. Среди элементов которой содержатся выпавшие узлы. Содержит в себе соответствующий новый номер узла, содержащийся в массиве B
		temp_B[i] = -1; // Обновленная индексация. По умолчанию считается, что все узлы выпали. Далее в этой функции проходит замена тех узлов, что не выпали на соответствующие им индексы, после упорядочивание и переиндексация. Содержит в себе соответствующий номер из нетронутой индексации. 
	}//дает возможность переходить от новой индексации к старой, и наоборот


	//Сортировка, основываясь на том, что строка удаленного элемента, в матрице узлов, содержит значения -1. По этому достаточно проверить только 1 элемент. Можно усложнить проверку: при наличии хотя бы одной ячейки со значением -1 строка удаляется.
	//------------------------------------------------------------------------------------------------------
	int n = 0; // кол-во удаленных строк
	for (int i = 0; (i + n) < temp_nElements; i++)
	{
		if (temp_inds[i+n][1] != -1) 
			for (int j = 0; j < 8; j++){
			temp_inds[i][j] = temp_inds[i + n][j];
			temp_B[temp_inds[i][j] - 1] = temp_inds[i][j] - 1;
			}
		else {
			i--;
			n++;
		}
	}
	temp_nElements = temp_nElements - n; //уменьшаем конечное число элементов. 

	//---------------------------------------------------------------------




	//Упорядочиваем индексацию. Что бы не было "пустых" номеров узлов		
	n = 0; // число выпавших узлов
	for (int i = 0; (i+n)< temp_nInds; i++)	
	{
		if (temp_B[i+n] != -1){ // загадочная часть, даже описать сложно. Как ни будь откомментирую
			temp_B[i] = temp_B[i + n]; 
			temp_A[temp_B[i]] = i; 
		}
		else {
			temp_A[i+n] = -1;
			n++;
			i--;
		}
	}
	temp_nInds = temp_nInds-n;

}

// нужно ровнять функцию сохранения. Создавать файл через интерфейс и записывать данные по алгоритму из текущей функции
void  myGLWidget::saveFile(char * to) 
{
    std::ofstream f;
	f.open("datasave.transf", std::ios::trunc);;//Открываем файл
	f << nElements << ' ';
	f << nInds << std::endl;

	for (int i = 0; i < nElements; i++){	
		for (int j = 0; j < 8; j++)
			f << inds[i][j]<< ' ';
		f << std::endl;
	}
	for (int i = 0; i < nInds; i++){
		for (int j = 0; j < 3; j++)
			f << koor[i][j]<< ' ';
		f << std::endl;
	}
	for (int i = 0; i < nInds; i++)
		f << potent[i]<< ' ';

	f.close();//Закрываем файл
}
