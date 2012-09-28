#include "svgview.h"
#include "svgscene.h"

SvgView::SvgView(QWidget *parent) :
    QGraphicsView(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //setFrameShape(QFrame::NoFrame);
    setRenderHint(QPainter::Antialiasing);
    setStyleSheet("background: transparent");

    setScene(new SvgScene(this));
}

void SvgView::resizeEvent(QResizeEvent *event)
{
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}
