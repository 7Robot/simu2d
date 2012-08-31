#include "SimulatorView.h"
#include "Simulator.h"
#include "Robot.h"

SimulatorView::SimulatorView(Simulator *simulator, QWidget *parent) :
    QGraphicsView(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFrameShape(QFrame::NoFrame);
    setRenderHint(QPainter::Antialiasing);

    QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    policy.setHeightForWidth(true);
    setSizePolicy(policy);

    scene = new SimulatorScene(simulator);
    simulator->scene = scene;
    setScene(scene);

    keyStates['Z'] = false;
    keyStates['Q'] = false;
    keyStates['S'] = false;
    keyStates['D'] = false;
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
    qreal sx = event->size().width()  / scene->sceneRect().width();
    qreal sy = event->size().height() / scene->sceneRect().height();
    qreal s = (sx < sy) ? sx : sy;
    resetMatrix();
    scale(s, s);
}

void SimulatorView::keyPressEvent(QKeyEvent * event)
{
    if(keyStates.contains(event->key())) {
        keyStates[event->key()] = true;
        scene->simulator->robot->KeyboardInput(keyStates);
    }
    else
        QWidget::keyPressEvent(event);
}

void SimulatorView::keyReleaseEvent(QKeyEvent * event)
{
    if(keyStates.contains(event->key())) {
        keyStates[event->key()] = false;
        scene->simulator->robot->KeyboardInput(keyStates);
    }
    else
        QWidget::keyReleaseEvent(event);
}
