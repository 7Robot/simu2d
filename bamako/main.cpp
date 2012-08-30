#include <QtGui>

#include "mainwindow.h"
#include "Simulator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bool headless = false;

    SimulatorScene *scene = NULL;
    if(!headless) {
        scene = new SimulatorScene();
    }

    Simulator simulator(scene);

    if(!headless) {
        scene->simulator = &simulator;
        simulator.updateScene();

        MainWindow *w = new MainWindow(scene);
        w->show();
    }

    simulator.start();

    return a.exec();
}
