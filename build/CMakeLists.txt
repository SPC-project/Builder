# Для сборки проекта перейдите в директорию Builder (родительская для этой)
#   и выполните в командной строке сначала 'cmake .' (под ОС Linux),
#   а потом 'make'
# Вы также можете собрать проект под Code::Blocks, выполнив 
#   'cmake . --DCMAKE_BUILD_TYPE=Debug' - это сформирует проект под эту IDE


# Переменные для сборки проекта
set(headers ../include)
set(resources ../res)
set(sources ../src)
file(GLOB code 
	${sources}/*.cpp	
)
# Необходимо явно подключить заголовочные файлы инструкцией add_executable
# 	в случае, если они лежат не в той же папке, что и CMakeLists.txt
file(GLOB code_h
	${headers}/*.h
)

include_directories(${headers} ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package( Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED )
if(NOT Qt4_FOUND)
    message(ERROR " Qt4 not found!")
endif(NOT Qt4_FOUND)
find_package( OpenGL REQUIRED )
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

qt4_wrap_ui(wrapped_ui ${resources}/opgl.ui)
qt4_add_resources(wrapped_res ${resources}/opgl.qrc)
INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES( ${QT_QTOPENGL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} )
ADD_DEFINITIONS(${QT_DEFINITIONS})

add_executable( ${prog_name} ${code} ${code_h} ${wrapped_ui} ${wrapped_res})
target_link_libraries( ${prog_name} ${QT_LIBRARIES} ${OPENGL_LIBRARIES})
