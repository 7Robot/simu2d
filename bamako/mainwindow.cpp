#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(SimulatorScene* scene, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    view = new SimulatorView(scene);
    ui->verticalLayout_2->addWidget(view);

}

MainWindow::~MainWindow()
{
    delete ui;
}
