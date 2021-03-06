#include <Window.h>
#include <QtGui>

//Часть кода, сгенерированного в дизайнере, находится в файле ui_opgl.h 
//Тем, кто хочет избежать кровотечения из глаз, советую открыть .ui в qtDesigner
Window::Window(){
	ui.setupUi(this);//подгружаем ui, в котором лежат все кнопочки и т.д.


	glLoaded = 0;

    // Указываем явно кодировку этого файла, чтобы Qt корректно отображала символы 
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("Utf-8"));

    ui.figure->setChecked(1);//чекбоксы
	ui.grid->setChecked(1);

	Input = new QString("*.transf");//ввод по умолчанию

	// Комбинации клавиш быстрого доступа
	QShortcut* openFile = new QShortcut(QKeySequence("Ctrl+O"), this);
	QShortcut* quitProgram = new QShortcut(QKeySequence("Ctrl+Q"), this);
	// Добавляем кнопочки на панель инструментов
	ui.mainToolBar->addWidget(ui.openFileTool);
	ui.mainToolBar->addWidget(ui.openDefaultTool);
	ui.mainToolBar->addSeparator();
	ui.mainToolBar->addWidget(ui.xLeft);
	ui.mainToolBar->addWidget(ui.xRight);
	ui.mainToolBar->addWidget(ui.yLeft);
	ui.mainToolBar->addWidget(ui.yRight);
	ui.mainToolBar->addWidget(ui.zLeft);
	ui.mainToolBar->addWidget(ui.zRight);
	ui.mainToolBar->addSeparator();
	ui.mainToolBar->addWidget(ui.pr1);
	ui.mainToolBar->addWidget(ui.pr2);
	ui.mainToolBar->addWidget(ui.pr3);
	ui.mainToolBar->addWidget(ui.pr4);
	ui.mainToolBar->addWidget(ui.pr5);
	ui.mainToolBar->addWidget(ui.pr6);
	ui.mainToolBar->addSeparator();
	ui.mainToolBar->addWidget(ui.zoomM);
	ui.mainToolBar->addWidget(ui.zoomP);
	ui.mainToolBar->addWidget(ui.snap);
	ui.mainToolBar->addSeparator();
	ui.mainToolBar->addWidget(ui.figure);
	ui.mainToolBar->addWidget(ui.air_part);
	ui.mainToolBar->addWidget(ui.grid);
	ui.mainToolBar->addWidget(ui.axis);
	ui.mainToolBar->addSeparator();
	ui.mainToolBar->addWidget(ui.author);
	ui.mainToolBar->addWidget(ui.cancel);
	ui.mainToolBar->addWidget(ui.exit);


	colorLine = new glLine();//строим линию градиента
	ui.glLineArea->setWidget(colorLine);//передаем ее в окно

	this->setFixedSize(947, 680);
	this->setWindowTitle(trUtf8("Builder"));

	// Shortcut's 
	connect(openFile,    SIGNAL(activated()), this, SLOT(openFile()));
	connect(quitProgram, SIGNAL(activated()), this, SLOT(close()));
	// Связываем кнопки с их функционалом
	connect(ui.pr1,            SIGNAL(clicked()), this, SLOT(getProection())  );
	connect(ui.pr2,            SIGNAL(clicked()), this, SLOT(getProection())  );
	connect(ui.pr3,            SIGNAL(clicked()), this, SLOT(getProection())  );
	connect(ui.pr4,            SIGNAL(clicked()), this, SLOT(getProection())  );
	connect(ui.pr5,            SIGNAL(clicked()), this, SLOT(getProection())  );
	connect(ui.pr6,            SIGNAL(clicked()), this, SLOT(getProection())  );
	connect(ui.xLeft,          SIGNAL(clicked()), this, SLOT(rotate())        );
	connect(ui.xRight,         SIGNAL(clicked()), this, SLOT(rotate())        );
	connect(ui.yLeft,          SIGNAL(clicked()), this, SLOT(rotate())        );
	connect(ui.yRight,         SIGNAL(clicked()), this, SLOT(rotate())        );
	connect(ui.zLeft,          SIGNAL(clicked()), this, SLOT(rotate())        );
	connect(ui.zRight,         SIGNAL(clicked()), this, SLOT(rotate())        );
	connect(ui.grid,           SIGNAL(clicked()), this, SLOT(changeMode())    );
    connect(ui.air_part,       SIGNAL(clicked()), this, SLOT(changeMode())    );
	connect(ui.figure,         SIGNAL(clicked()), this, SLOT(changeMode())    );
	connect(ui.axis,           SIGNAL(clicked()), this, SLOT(changeMode())    );
	connect(ui.snap,           SIGNAL(clicked()), this, SLOT(SnapShot())      );
	connect(ui.zoomP,          SIGNAL(clicked()), this, SLOT(zoom())          );
	connect(ui.zoomM,          SIGNAL(clicked()), this, SLOT(zoom())          );
	connect(ui.openFileTool,   SIGNAL(clicked()), this, SLOT(openFile())      );
	connect(ui.openDefaultTool,SIGNAL(clicked()), this, SLOT(createGlWidget()));
	connect(ui.exit,           SIGNAL(clicked()), this, SLOT(close())         );
	connect(ui.cancel,         SIGNAL(clicked()), this, SLOT(delGL())         );
	connect(ui.author,         SIGNAL(clicked()), this, SLOT(showAutor())     );
}


void Window::keyPressEvent(QKeyEvent* pe)
{//обработчик событий клавиатуры
	if (glLoaded)
	{
		switch (pe->key())
		{
		case Qt::Key_Plus:
			glWidget->zoom(1);
			break;

		case Qt::Key_Equal:
			glWidget->zoom(1);
			break;

		case Qt::Key_Minus:
			glWidget->zoom(0);
			break;

		case Qt::Key_Up:
			glWidget->see_y(1);
			break;

		case Qt::Key_Down:
			glWidget->see_y(0);
			break;

		case Qt::Key_Left:
			glWidget->move_only_cam_x(1);
			break;

		case Qt::Key_Right:
			glWidget->move_only_cam_x(0);
			break;

		case Qt::Key_W:
			glWidget->rotate_y(1, 0.5f);
			break;

		case Qt::Key_S:
			glWidget->rotate_y(0, 0.5f);
			break;

		case Qt::Key_A:
			glWidget->rotate_x(0, 0.5f);
			break;

		case Qt::Key_D:
			glWidget->rotate_x(1, 0.5f);
			break;

		case Qt::Key_Q:
			glWidget->rotate_z(0, 0.5f);
			break;

		case Qt::Key_E:
			glWidget->rotate_z(1, 0.5f);
			break;

		case Qt::Key_T:
			glWidget->see_y(0);
			break;

		case Qt::Key_G:
			glWidget->see_y(1);
			break;

		case Qt::Key_F:
			glWidget->move_only_cam_x(0);
			break;

		case Qt::Key_H:
			glWidget->move_only_cam_x(1);
			break;

		case Qt::Key_Escape:
			this->close();
			break;

		case Qt::Key_Space:
			glWidget->setStandartLook();
			break;

        case Qt::Key_Delete:
			glWidget->deleteArea();
			break;

		case Qt::Key_Z:
			glWidget->saveFile(Input->toUtf8().data());
			break;

		case Qt::Key_1:
			glWidget->getProection(1);
			break;
		case Qt::Key_2:
			glWidget->getProection(2);
			break;
		case Qt::Key_3:
			glWidget->getProection(3);
			break;
		case Qt::Key_4:
			glWidget->getProection(4);
			break;
		case Qt::Key_5:
			glWidget->getProection(5);
			break;
		case Qt::Key_6:
			glWidget->getProection(6);
			break;
		}

		glWidget->updateGL();
	}
}

void Window::zoom(){
    if (glLoaded) {
		//находим, от какой из кнопок отправлен сигнал и приближаем/отдаляем
		if (this->sender() == ui.zoomP)
			glWidget->zoom(1);
		else if (this->sender() == ui.zoomM)
			glWidget->zoom(0);
    }
}

void Window::rotate(){
    if (glLoaded) {
		//тоже находим кнопку-отправителя сигнала
		if (this->sender() == ui.xLeft)
			glWidget->rotate_x(0, 1);
		else if (this->sender() == ui.xRight)
			glWidget->rotate_x(1, 1);
		else if (this->sender() == ui.yLeft)
			glWidget->rotate_y(1, 1);
		else if (this->sender() == ui.yRight)
			glWidget->rotate_y(0, 1);
		else if (this->sender() == ui.zLeft)
			glWidget->rotate_z(0, 1);
		else if (this->sender() == ui.zRight)
			glWidget->rotate_z(1, 1);
    }
}

void Window::getProection(){
    if (glLoaded) {
		//ну вы поняли
		if (this->sender() == ui.pr1)
			glWidget->getProection(1);
		else if (this->sender() == ui.pr2)
			glWidget->getProection(2);
		else if (this->sender() == ui.pr3)
			glWidget->getProection(3);
		else if (this->sender() == ui.pr4)
			glWidget->getProection(4);
		else if (this->sender() == ui.pr5)
			glWidget->getProection(5);
		else if (this->sender() == ui.pr6)
            glWidget->getProection(6);\
    }
}

void Window::changeMode(){
	if (glLoaded){
        // TODO: установить, зачем проверять перед присваиванием?
		//выставляем флаги прорисовки
		if (ui.grid->isChecked() != glWidget->drawgr)
			glWidget->drawgr = ui.grid->isChecked();

		if (ui.figure->isChecked() != glWidget->drawfig)
			glWidget->drawfig = ui.figure->isChecked();

		if (ui.axis->isChecked() != glWidget->drawax)
			glWidget->drawax = ui.axis->isChecked();

        if (ui.air_part->isChecked() != glWidget->drawair)
            glWidget->drawair = ui.air_part->isChecked();

		//if (ui.points->isChecked() != glWidget->drawdot)
		//glWidget->drawdot = ui.points->isChecked();


		glWidget->updateGL();
	}
}

void Window::SnapShot(){
	if (glLoaded){
		glWidget->paintGL();//прорисовываем сцену еще разочек
		QImage i = glWidget->grabFrameBuffer();//копируем буффер в картинку
		QString str = QFileDialog::getSaveFileName(this, trUtf8("Save File"), "",
			trUtf8(" Images (*.png; *.bmp; *.jpg)"));//спрашиваем куда сохраняем
		QString format;
		if (str.endsWith("jpg", Qt::CaseInsensitive))//определяем, в каком формате
			format = "JPEG";
		if (str.endsWith("bmp", Qt::CaseInsensitive))
			format = "BMP";
		if (str.endsWith("png", Qt::CaseInsensitive))
			format = "PNG";

		i.save(str, format.toUtf8().data());//сохраняем
	}
}

void Window::openFile(){

	QString fname = QFileDialog::getOpenFileName(this, trUtf8("Открыть"), "",
		trUtf8("Transformer Files (*.transf)"));//спрашиваем, из какого файла читаем
	if (fname != ""){//если нам указали путь
		Input = new QString(fname);//запоминаем его
		createGlWidget();//строим gl 
	}
}

void Window::createGlWidget(){

	if (glLoaded)//если уже что-то построили - удаляем
		delete glWidget;

	glWidget = new myGLWidget;//создаем новый виджет
	int status = glWidget->readFile(Input->toUtf8().data());//грузим фигуру, какую скажут (или по умолчанию)
    if( status != 0 ){
        return;
    }
	ui.glWidgetArea->setWidget(glWidget);//передаем виджет в окно
	glWidget->updateGL();//рисуем

	ui.blueValue->setNum(glWidget->minPotent);//подписываем значения цветов
	ui.redValue->setNum(glWidget->maxPotent);
	ui.greenValue->setNum((glWidget->maxPotent - glWidget->minPotent) / 2 + glWidget->minPotent);
	glLoaded = 1;//виджет загружен 
}

void Window::delGL(){
	if (glLoaded)
		delete glWidget;//сносим виджжет
	ui.blueValue->setText("");//удаляем подписи на линии градиентов
	ui.redValue->setText("");
	ui.greenValue->setText("");

	glLoaded = 0;//виджета нету :(
}

void Window::showAutor(){
	//Собственно, ваш покорный слуга
	QMessageBox::information(this, trUtf8("Об Авторе"),
		trUtf8("Автор программы: Ермаков Даниэль.\n"
		"  E-Mail: dan-ermakov@rambler.ru\n"
		"        НТУ ХПИ. Кафедра СПУ. \n"
		"                 Харьков 2014"), QMessageBox::Cancel);

}
