#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QGraphicsScene* scene, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene->setSceneRect(scene->sceneRect()); // The bounding box won't grow.
    qreal sx = (ui->graphicsView->width() - 2) / scene->sceneRect().width();
    qreal sy = (ui->graphicsView->height() - 2) / scene->sceneRect().height();
    qreal s = (sx < sy) ? sx : sy;

    ui->graphicsView->scale(s, s);
    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}
