#include "SimulatorScene.h"
#include "Simulator.h"



SimulatorScene::SimulatorScene(Simulator *simulator, QWidget *parent) :
    QGraphicsScene(parent), simulator(simulator), mouseJoint(NULL)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setSceneRect(-1.50, -1.0, 3.0, 2.0);
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
}

void SimulatorScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    b2Vec2 p;
    p.Set(mouseEvent->scenePos().x(),
          mouseEvent->scenePos().y());

    if(mouseJoint) {
        mouseJoint->SetTarget(p);
    }
}

void SimulatorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    if(mouseJoint) {
        simulator->world->DestroyJoint(mouseJoint);
        mouseJoint = NULL;
    }
}
