#include "SimulatorScene.h"
#include "Simulator.h"


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

SimulatorScene::SimulatorScene(QWidget *parent) :
    QGraphicsScene(parent), mouseJoint(NULL)
{
}

void SimulatorScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    mousePos.Set(mouseEvent->scenePos().x(),
                 mouseEvent->scenePos().y());

    if (mouseJoint != NULL) {
        return;
    }

    // Make a small box.
    b2AABB aabb;
    b2Vec2 d;
    d.Set(0.001f, 0.001f);
    aabb.lowerBound = mousePos - d;
    aabb.upperBound = mousePos + d;

    // Query the world for overlapping shapes.
    QueryCallback callback(mousePos);
    simulator->world->QueryAABB(&callback, aabb);

    if (callback.fixture) {
        b2Body* body = callback.fixture->GetBody();
        b2MouseJointDef md;
        md.bodyA = simulator->groundBody;
        md.bodyB = body;
        md.target = mousePos;
        md.maxForce = 1000.0f * body->GetMass();
        mouseJoint = (b2MouseJoint*)simulator->world->CreateJoint(&md);
        body->SetAwake(true);
    }

    //QGraphicsScene::mousePressEvent(mouseEvent);
}

void SimulatorScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    b2Vec2 p;
    p.Set(mouseEvent->scenePos().x(),
          mouseEvent->scenePos().y());

    if(mouseJoint) {
        mouseJoint->SetTarget(p);
    }

    //QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void SimulatorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if(mouseJoint) {
        simulator->world->DestroyJoint(mouseJoint);
        mouseJoint = NULL;
    }

    //QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
