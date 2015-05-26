#include "glLine.h"
#include "ui_opgl.h"

class Window : public QMainWindow {
	Q_OBJECT

public:
	Window();


	public slots:
	void          zoom();
	void          rotate();
	void          getProection();
	void          changeMode();
	void          SnapShot();
	void          openFile();
	void          createGlWidget();
	void          delGL();
	void          showAutor();



protected:
	virtual void keyPressEvent(QKeyEvent* pe);

private:
	Ui::ToolsPanelClass ui;
	bool         glLoaded;//флаг существований gl виджета


	myGLWidget*  glWidget;
	glLine*      colorLine;

	QString*     Input;

};
