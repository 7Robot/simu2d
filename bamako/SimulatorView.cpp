#include "SimulatorView.h"

SimulatorView::SimulatorView(SimulatorScene *scene, QWidget *parent) :
    QGraphicsView(scene, parent), scene(scene)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFrameShape(QFrame::NoFrame);

    QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    policy.setHeightForWidth(true);
    setSizePolicy(policy);
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
