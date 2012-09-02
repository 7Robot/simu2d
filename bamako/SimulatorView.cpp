#include "SimulatorView.h"
#include "Simulator.h"
#include "Robot.h"

SimulatorView::SimulatorView(QWidget *parent) :
    QGraphicsView(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFrameShape(QFrame::NoFrame);
    setRenderHint(QPainter::Antialiasing);
    setStyleSheet("background: transparent");

    keyStates['Z'] = false;
    keyStates['Q'] = false;
    keyStates['S'] = false;
    keyStates['D'] = false;
}

void SimulatorView::setSimulator(Simulator *simulator)
{
    SimulatorScene *scene = new SimulatorScene(simulator);
    setScene(scene);
    simulator->scene = scene;
}

SimulatorScene * SimulatorView::scene()
{
    return (SimulatorScene*)QGraphicsView::scene();
}

int SimulatorView::heightForWidth(int w) const
{
    return w * 2.0 / 3.0;
}

QSize SimulatorView::sizeHint() const
{
    int w = 300;
    return QSize(w, heightForWidth(w));
}

void SimulatorView::resizeEvent(QResizeEvent *event)
{
    QRectF rect(0, 0, 300, 200);
    if(scene())
        rect  = scene()->sceneRect();
    qreal sx = event->size().width()  / rect.width();
    qreal sy = event->size().height() / rect.height();
    qreal s = (sx < sy) ? sx : sy;
    resetMatrix();
    scale(s, s);
}

void SimulatorView::keyPressEvent(QKeyEvent * event)
{
    if(keyStates.contains(event->key())) {
        keyStates[event->key()] = true;
        if(scene())
            scene()->simulator->robot->KeyboardInput(keyStates);
    }
    else
        QWidget::keyPressEvent(event);
}

void SimulatorView::keyReleaseEvent(QKeyEvent * event)
{
    if(keyStates.contains(event->key())) {
        keyStates[event->key()] = false;
        if(scene())
            scene()->simulator->robot->KeyboardInput(keyStates);
    }
    else
        QWidget::keyReleaseEvent(event);
}
