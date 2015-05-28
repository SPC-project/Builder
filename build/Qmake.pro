QT += core gui opengl

TARGET = Builder

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += ../include

SOURCES += ../src/main.cpp
SOURCES += ../src/glLine.cpp
SOURCES += ../src/MyGlWidget.cpp
SOURCES += ../src/Window.cpp

HEADERS += ../include/glLine.h
HEADERS += ../include/MyGlWidget.h
HEADERS += ../include/Window.h

FORMS = ../res/opgl.ui
RESOURCES = ../res/opgl.qrc

unix:LIBS += -lGLU
win32:LIBS += glu32
