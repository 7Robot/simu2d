#include "Simulator.h"
#include "SimulatorScene.h"


Simulator::Simulator(SimulatorScene *scene, QObject *parent)
    : QObject(parent), scene(scene), timerId(0), gravity(9.8), time(0)
{
    world = new b2World(b2Vec2(0.0f, 0.0f)); // Zero gravity.
    populate();
}

void Simulator::populate()
{
    b2BodyDef bodyDef;
    b2PolygonShape polygonShape;
    b2CircleShape circleShape;
    b2ChainShape chainShape;
    b2FixtureDef fixtureDef;

    // Ground.
    bodyDef.position.Set(0.0f, 0.0f);
    groundBody = world->CreateBody(&bodyDef);
    polygonShape.SetAsBox(1.5f, 1.0f);
    fixtureDef.isSensor = true;
    addPolygon(groundBody, polygonShape, fixtureDef, Qt::blue);

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
    bodyDef.position.Set(1.0f, 0.5f);
    robotBody = world->CreateBody(&bodyDef);
    polygonShape.SetAsBox(0.2f, 0.2f);
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    addPolygon(robotBody, polygonShape, fixtureDef, Qt::gray);
    addFriction(robotBody);

    // Coin.
    bodyDef.position.Set(0.0f, 0.0f);
    b2Body *coinBody = world->CreateBody(&bodyDef);
    circleShape.m_radius = 0.06f;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    addCircle(coinBody, circleShape, fixtureDef, Qt::white);
    circleShape.m_radius = 0.01f;
    addCircle(coinBody, circleShape, fixtureDef, Qt::blue);
    addFriction(coinBody);
}

void Simulator::addFriction(b2Body *body)
{
    float32 mass = body->GetMass();
    float32 I = body->GetInertia();

    b2FrictionJointDef jd;
    jd.bodyA = groundBody;
    jd.bodyB = body;
    jd.collideConnected = true;
    jd.maxForce = mass * gravity;
    jd.maxTorque = I * gravity; // TODO

    world->CreateJoint(&jd);
}

void Simulator::addPolygon(b2Body *body, b2PolygonShape &shape, b2FixtureDef &fixtureDef, QColor color)
{
    fixtureDef.shape = &shape;
    b2Fixture* fixture = body->CreateFixture(&fixtureDef);

    if(scene != NULL) {
        QPolygonF polygon;
        for(int i = 0; i < shape.GetVertexCount(); i++) {
            b2Vec2 vertex =  shape.GetVertex(i);
            polygon << QPointF(vertex.x, vertex.y);
        }

        QGraphicsPolygonItem *item = scene->addPolygon(polygon, QPen(Qt::NoPen), QBrush(color));
        fixture->SetUserData((void*)item);
    }
}

void Simulator::addCircle(b2Body *body, b2CircleShape &shape, b2FixtureDef &fixtureDef, QColor color)
{
    fixtureDef.shape = &shape;
    b2Fixture* fixture = body->CreateFixture(&fixtureDef);

    if(scene != NULL) {
        QGraphicsEllipseItem *item = scene->addEllipse(-shape.m_radius,   -shape.m_radius,
                                                       2 * shape.m_radius, 2 * shape.m_radius,
                                                       QPen(Qt::NoPen), QBrush(color));
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
        motionControlStep();
        world->Step(B2_TIMESTEP, B2_VELOCITY_ITERATIONS, B2_POSITION_ITERATIONS);
        updateScene();
    }
    QObject::timerEvent(event);
}

void Simulator::motionControlStep()
{
/*    float velChange = desiredVel - vel.x;
    float impulse = body->GetMass() * velChange; //disregard time factor
    robotBody->ApplyLinearImpulse(b2Vec2(impulse, 0).,
                                  robotBody->GetWorldCenter());
    robotBody->ApplyLinearImpulse(impulse, robotBody->GetWorldCenter());
*/
}
