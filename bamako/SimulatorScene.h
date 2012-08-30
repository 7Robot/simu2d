#ifndef SIMULATORVIEW_H
#define SIMULATORVIEW_H

#include <QtGui>
#include <Box2D.h>


class Simulator;
class Q_GUI_EXPORT SimulatorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SimulatorScene(QWidget *parent = 0);
    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
    Simulator *simulator;
    
private:
    b2Vec2 mousePos;
    b2MouseJoint *mouseJoint;

signals:
    
public slots:
    
};


#endif // SIMULATORVIEW_H
