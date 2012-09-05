#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "qcustomplot.h"
#include "Robot.h"

MainWindow::MainWindow(Simulator* simulator, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), time(0)
{
    ui->setupUi(this);

    ui->simulatorView->setSimulator(simulator);
    simulator->mainWindow = this;

    ui->plot->addGraph();
    ui->plot->addGraph();
    ui->plot->graph(1)->setPen(QPen(Qt::red));
    //ui->plot->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    ui->plot->setRangeZoom(Qt::Horizontal);
}

void MainWindow::plotStep(double a0, double a1)
{
    QCPGraph * graph0 = ui->plot->graph(0);
    QCPGraph * graph1 = ui->plot->graph(1);
    graph0->addData(time, a0);
    graph1->addData(time, a1);

    graph0->keyAxis()->moveRange(time - graph0->keyAxis()->range().upper); // Right-align.
    double lower = graph0->keyAxis()->range().lower;
    graph0->removeDataBefore(lower);
    graph1->removeDataBefore(lower);
    graph0->rescaleValueAxis();
    graph1->rescaleValueAxis(true); // Only enlarge.

    ui->plot->replot();
    time += B2_TIMESTEP;
}

void MainWindow::pause()
{
    if(!ui->simulatorView->scene()->simulator->stop())
        ui->simulatorView->scene()->simulator->start();
}

void MainWindow::plusDist()
{
    ui->simulatorView->scene()->simulator->robot->distanceControl = ControlSetpoint;
    ui->simulatorView->scene()->simulator->robot->distanceSetpoint = .5;
}

void MainWindow::minusDist()
{
    ui->simulatorView->scene()->simulator->robot->distanceControl = ControlSetpoint;
    ui->simulatorView->scene()->simulator->robot->distanceSetpoint = -.5;
}

void MainWindow::plusRot()
{
    ui->simulatorView->scene()->simulator->robot->angleControl = ControlSetpoint;
    ui->simulatorView->scene()->simulator->robot->angleSetpoint = PI/3;
}

void MainWindow::minusRot()
{
    ui->simulatorView->scene()->simulator->robot->angleControl = ControlSetpoint;
    ui->simulatorView->scene()->simulator->robot->angleSetpoint = -PI/3;
}

MainWindow::~MainWindow()
{
    delete ui;
}
