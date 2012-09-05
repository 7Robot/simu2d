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
    void plotStep(double a0, double a1 = 0);
    ~MainWindow();

public slots:
    void pause();

    void plusDist();
    void minusDist();
    void plusRot();
    void minusRot();


private:
    Ui::MainWindow *ui;
    double time;
};

#endif // MAINWINDOW_H
