
#include "Simulator.h"


Simulator::Simulator(SimulatorScene *scene, QObject *parent)
        : QObject(parent), scene(scene), timerId(0)
{
    world = new b2World(b2Vec2(0.0f, 1.0f)); // Zero gravity.


    b2BodyDef bodyDef;
    b2PolygonShape polygonShape;
    b2ChainShape chainShape;
    b2FixtureDef fixtureDef;

    // Ground.
    bodyDef.position.Set(0.0f, 0.0f);
    groundBody = world->CreateBody(&bodyDef);
    polygonShape.SetAsBox(1.5f, 1.0f);
    fixtureDef.isSensor = true;
    addPolygon(groundBody, polygonShape, fixtureDef);

    // Ground borders.
    b2Vec2 vs[4];
    for(int i = 0; i < 4; i++) {
        vs[i] = polygonShape.GetVertex(i);
    }
    chainShape.CreateLoop(vs, 4);
    fixtureDef.isSensor = false;
    fixtureDef.shape = &chainShape;
    fixtureDef.userData = NULL;
    groundBody->CreateFixture(&fixtureDef);

    // Robot.
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 0.0f);
    robotBody = world->CreateBody(&bodyDef);
    polygonShape.SetAsBox(0.2f, 0.2f);
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    addPolygon(robotBody, polygonShape, fixtureDef);
}

void Simulator::addPolygon(b2Body *body, b2PolygonShape &polygonShape, b2FixtureDef &fixtureDef)
{
    fixtureDef.shape = &polygonShape;
    b2Fixture* fixture = body->CreateFixture(&fixtureDef);

    if(scene != NULL) {
        QPolygonF polygon;
        for(int i = 0; i < polygonShape.GetVertexCount(); i++) {
            b2Vec2 vertex =  polygonShape.GetVertex(i);
            polygon << QPointF(vertex.x, vertex.y);
        }

        QGraphicsPolygonItem *item = scene->addPolygon(polygon, QPen(Qt::NoPen), QBrush(Qt::blue));
        item->setOpacity(.4);
        fixture->SetUserData((void*)item);
    }
}

void Simulator::start()
{
    if (!timerId)
        timerId = startTimer(B2_TIMESTEP * 1000);
}

Simulator::~Simulator()
{
    delete world;
}

void Simulator::updateScene() // Refresh the interface.
{
    for (b2Body* body = world->GetBodyList(); body; body = body->GetNext()) {
        if(body->IsAwake()) {
            // FIXME check that the position changed significantly for less overhead.

            b2Vec2 position = body->GetPosition();
            float32 angle = body->GetAngle();
            for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
            {
                void* userdata = f->GetUserData();

                if(userdata != NULL) {
                    QGraphicsItem* item = (QGraphicsItem*)userdata;
                    item->setPos(position.x, position.y);
                    item->setRotation(angle * 180.0 / PI);
                }
            }
        }
    }
}

void Simulator::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerId) {
        world->Step(B2_TIMESTEP, B2_VELOCITY_ITERATIONS, B2_POSITION_ITERATIONS);
        // FIXME One refresh for multiple steps.

        if(scene != NULL)
            updateScene();
    }
    QObject::timerEvent(event);
}
