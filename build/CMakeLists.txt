# Для сборки проекта перейдите в директорию Builder (родительская для этой)
#   и выполните в командной строке сначала 'cmake .' (под ОС Linux),
#   а потом 'make'
# Вы также можете собрать проект под Code::Blocks, выполнив 
#   'cmake . --DCMAKE_BUILD_TYPE=Debug' - это сформирует проект под эту IDE

cmake_minimum_required(VERSION 2.8.7)  # Поверка версии cmake
project(Builder)

# Определяем флаги сборки
set(DEBUG_FLAGS "-g -Og -pipe -Wall -pedantic -std=gnu++11")
set(RELEASE_FLAGS "-O2 -pipe -std=gnu++11 -march=native")
set(EXECUTABLE_OUTPUT_PATH ../bin)

# Определяем параметры сборки
set(prog_name builder)
set(CMAKE_CXX_FLAGS ${RELEASE_FLAGS})
set(CMAKE_C_FLAGS ${RELEASE_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG ${DEBUG_FLAGS})
set(CMAKE_C_FLAGS_DEBUG ${DEBUG_FLAGS})
set(CMAKE_CONFIGURATION_TYPES Debug Release)

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

IF(WIN32) # Check if we are on Windows
  if(MSVC) # Check if we are using the Visual Studio compiler
    set_target_properties(${prog_name} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
  else()
    message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
  endif()
elseif(UNIX)
  # Nothing special required
else()
  message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
ENDIF()

# 'make run' - собрать и запустить
add_custom_target(run
    COMMAND ${prog_name}
    DEPENDS ${prog_name}
    WORKING_DIRECTORY ../bin
)
