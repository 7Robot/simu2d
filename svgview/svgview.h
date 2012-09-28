#ifndef SVGVIEW_H
#define SVGVIEW_H

#include <QGraphicsView>
#include <QResizeEvent>

class SvgView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SvgView(QWidget *parent = 0);
    void resizeEvent(QResizeEvent *event);
};

#endif // SVGVIEW_H
