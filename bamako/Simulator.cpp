#include "Simulator.h"
#include "SimulatorScene.h"
#include "Robot.h"
#include "MainWindow.h"
#include "Box2D/Box2D.h"
#include <QtSvg>

Simulator::Simulator(QObject *parent)
    : QObject(parent), mainWindow(NULL), scene(NULL), timerId(0), gravity(9.8), time(0)
{
    world = new b2World(b2Vec2(0.0f, 0.0f)); // Zero gravity.
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
    addSvgPolygon(groundBody, polygonShape, fixtureDef);

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
    bodyDef.position.Set(1.2f, 0.7f);
    bodyDef.angle = PI;
    robotBody = world->CreateBody(&bodyDef);
    polygonShape.SetAsBox(0.2f, 0.2f);
    fixtureDef.density = 1.5f;
    fixtureDef.friction = 0.9f;
    addPolygon(robotBody, polygonShape, fixtureDef, Qt::red);
    addFriction(robotBody);
    robot = new Robot(this, robotBody);

    // Opponent.
    bodyDef.position.Set(-1.2f, 0.7f);
    bodyDef.angle = 0.0f;
    opponentBody = world->CreateBody(&bodyDef);
    addPolygon(opponentBody, polygonShape, fixtureDef, Qt::blue);
    addFriction(opponentBody);
    opponent = new Robot(this, opponentBody);

    // Coin.
    bodyDef.position.Set(-1.f, -1.f);
    b2Body *coinBody = world->CreateBody(&bodyDef);
    circleShape.m_radius = 0.06f;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    addCircle(coinBody, circleShape, fixtureDef, Qt::white);
    circleShape.m_radius = 0.01f;
    addCircle(coinBody, circleShape, fixtureDef, Qt::blue);
    addFriction(coinBody);

    updateScene();
}

void Simulator::addSvgPolygon(b2Body *body, b2PolygonShape &shape, b2FixtureDef &fixtureDef)
{
    fixtureDef.shape = &shape;
    b2Fixture* fixture = body->CreateFixture(&fixtureDef);

    if(scene != NULL) {
        QGraphicsSvgItem *item = new QGraphicsSvgItem();
        item->setSharedRenderer(scene->renderer);
        item->setElementId("background"); // TODO

        scene->addItem(item);
        fixture->SetUserData((void*)item);
    }
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

void Simulator::addFriction(b2Body *body)
{
    float32 mass = body->GetMass();
    //float32 I = body->GetInertia();

    b2FrictionJointDef jd;
    jd.bodyA = groundBody;
    jd.bodyB = body;
    jd.collideConnected = true;
    jd.maxForce = mass * gravity;
    jd.maxTorque = .2 * mass * gravity;

    world->CreateJoint(&jd);
}

void Simulator::start()
{
    if (!timerId)
        timerId = startTimer(B2_TIMESTEP * 1000);
}

bool Simulator::stop()
{
    bool tmp = timerId;
    killTimer(timerId);
    timerId = 0;
    return tmp;
}

Simulator::~Simulator()
{
    delete world;
}

void Simulator::updateScene() // Refresh the interface.
{
    if(scene == NULL)
        return;

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
                    //item->setRotation(angle * 180.0 / PI); // TODO
                }
            }
        }
    }
}

void Simulator::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerId) {
        robot->preStep();
        opponent->preStep();

        // World simulation.
        world->Step(B2_TIMESTEP, B2_VELOCITY_ITERATIONS, B2_POSITION_ITERATIONS);

        robot->postStep();
        opponent->postStep();

        b2Vec2 pos = robot->position - robot->positionOffset;
        mainWindow->debugString(QString("x=%1  y=%2").arg(pos.x).arg(pos.y));

        updateScene();
    }
    QObject::timerEvent(event);
}
