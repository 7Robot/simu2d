#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "SimulatorView.h"
#include "Simulator.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Simulator *simulator, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SimulatorView *view;
};

#endif // MAINWINDOW_H
