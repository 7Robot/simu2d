#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QtGui>
#include <Box2D.h>

static const float32 B2_TIMESTEP = 1.0f / 60.0f;
static const int32 B2_VELOCITY_ITERATIONS = 6;
static const int32 B2_POSITION_ITERATIONS = 2;
static const float PI = 3.14159265359;

class SimulatorScene;

enum MotionControlType
{
    FreerunControl  = 0,
    AngleControl    = 0x1,
    DistanceControl = 0x2,
    PolarControl    = 0x3,
    SpeedControl    = 0x4
};

class Simulator : public QObject
{
    Q_OBJECT
public:
    Simulator(SimulatorScene *scene, QObject *parent = 0);

    void populate();
    void addPolygon(b2Body *body, b2PolygonShape &shape, b2FixtureDef &fixtureDef, QColor color);
    void addCircle (b2Body *body, b2CircleShape  &shape, b2FixtureDef &fixtureDef, QColor color);
    void addFriction(b2Body *body);
    void start();

    ~Simulator();

    void updateScene();
    void motionControlStep();
    b2World *world;

    b2Body* groundBody;
    b2Body* robotBody;

    float angleSetpoint;
    float distanceSetpoint;
    float omegaLimit;
    float speedLimit;
    MotionControlType motionControl;

protected:
    void timerEvent(QTimerEvent *event);

private:
    SimulatorScene *scene;
    int timerId;
    float gravity;
    int time;

    // Les consignes arriveront dans le signal QTcpSocket::readyRead().
};

#endif // SIMULATOR_H
