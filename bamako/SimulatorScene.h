#ifndef SIMULATORSCENE_H
#define SIMULATORSCENE_H

#include <QtGui>
#include <Box2D.h>

class Simulator;

class SimulatorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SimulatorScene(Simulator* simulator, QWidget *parent = 0);
    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
    Simulator *simulator;
    
private:
    b2Vec2 mousePos;
    b2MouseJoint *mouseJoint;
};

class QueryCallback : public b2QueryCallback // From Box2D's TestBed.
{
public:
    QueryCallback(const b2Vec2& point) : point(point), fixture(NULL)
    { }

    bool ReportFixture(b2Fixture* f)
    {
        b2Body* body = f->GetBody();
        if (body->GetType() == b2_dynamicBody) {
            bool inside = f->TestPoint(point);
            if (inside) {
                fixture = f;
                return false; // We are done, terminate the query.
            }
        }
        return true; // Continue the query.
    }

    b2Vec2 point;
    b2Fixture* fixture;
};

#endif // SIMULATORSCENE_H
