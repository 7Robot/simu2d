#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(Simulator* simulator, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    view = new SimulatorView(simulator);
    ui->verticalLayout->addWidget(view);
}

MainWindow::~MainWindow()
{
    delete ui;
}
