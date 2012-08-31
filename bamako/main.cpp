#include <QtGui>

#include "MainWindow.h"
#include "SimulatorView.h"
#include "SimulatorScene.h"
#include "Simulator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Simulator simulator;
    bool headless = false;

    QStringList args = a.arguments();
    for(int i = 1; i < args.size(); i++) {
        if(args.at(i).startsWith("--headless")) {
            headless = true;
        }
    }


    if(headless) {
        simulator.populate();
        simulator.start();
    }
    else {
        MainWindow *w = new MainWindow(&simulator);
        simulator.populate(); // Will create some widgets.
        w->show();
        simulator.start();
    }

    return a.exec();
}
