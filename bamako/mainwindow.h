#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "SimulatorView.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(SimulatorScene *scene, QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    SimulatorView *view;
};

#endif // MAINWINDOW_H
