# http://qt-project.org/quarterly/view/using_cmake_to_build_qt_projects
# http://qt-quarterly.developpez.com/qq-34/cmake/

cmake_minimum_required(VERSION 2.6.0)

project(Simu2D)

add_subdirectory(Box2D_v2.2.1)
include_directories(${Box2D_SOURCE_DIR})

find_package(Qt4 REQUIRED)

# librairies (core & gui already included)
#set(QT_DONT_USE_QTCORE TRUE)
#set(QT_DONT_USE_QTGUI TRUE)
#set(QT_USE_QT3SUPPORT TRUE)
#set(QT_USE_QTASSISTANT TRUE)
#set(QT_USE_QAXCONTAINER TRUE)
#set(QT_USE_QAXSERVER TRUE)
#set(QT_USE_QTDESIGNER TRUE)
#set(QT_USE_QTMOTIF TRUE)
#set(QT_USE_QTMAIN TRUE)
#set(QT_USE_QTNETWORK TRUE)
#set(QT_USE_QTNSPLUGIN TRUE)
#set(QT_USE_QTOPENGL TRUE)
#set(QT_USE_QTSQL TRUE)
#set(QT_USE_QTXML TRUE)
set(QT_USE_QTSVG TRUE)
#set(QT_USE_QTTEST TRUE)
#set(QT_USE_QTUITOOLS TRUE)
#set(QT_USE_QTDBUS TRUE)
#set(QT_USE_QTSCRIPT TRUE)
#set(QT_USE_QTASSISTANTCLIENT TRUE)
#set(QT_USE_QTHELP TRUE)
#set(QT_USE_QTWEBKIT TRUE)
#set(QT_USE_QTXMLPATTERNS TRUE)
#set(QT_USE_PHONON TRUE)

#include_directories(${CMAKE_CURRENT_BINARY_DIR})
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

file(GLOB simu2d_SOURCES "*.cpp")
set(simu2d_HEADERS MainWindow.h Simulator.h SimulatorScene.h SimulatorView.h
    qcustomplot.h)
set(simu2d_FORMS MainWindow.ui)
set(simu2d_RESOURCES res.qrc)

QT4_WRAP_CPP(simu2d_HEADERS_MOC ${simu2d_HEADERS})
QT4_WRAP_UI(simu2d_FORMS_HEADERS ${simu2d_FORMS})
QT4_ADD_RESOURCES(simu2d_RESOURCES_RCC ${simu2d_RESOURCES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(.)

add_executable(simu2d
    ${simu2d_SOURCES}
    ${simu2d_HEADERS_MOC} 
	${simu2d_FORMS_HEADERS} 
	${simu2d_RESOURCES_RCC})

target_link_libraries(simu2d ${QT_LIBRARIES})
target_link_libraries(simu2d Box2D)

install(TARGETS simu2d RUNTIME DESTINATION bin)
