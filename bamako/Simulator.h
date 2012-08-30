#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QtGui>
#include <Box2D.h>

static const float32 B2_TIMESTEP = 1.0f / 60.0f;
static const int32 B2_VELOCITY_ITERATIONS = 6;
static const int32 B2_POSITION_ITERATIONS = 2;
static const float PI = 3.14159265359;

class SimulatorScene;

class Simulator : public QObject
{
    Q_OBJECT
public:
    Simulator(SimulatorScene *scene, QObject *parent = 0);

    void addPolygon(b2Body *body, b2PolygonShape &shape, b2FixtureDef &fixtureDef, QColor color);
    void addCircle (b2Body *body, b2CircleShape  &shape, b2FixtureDef &fixtureDef, QColor color);
    void start();

    ~Simulator();

    void updateScene();
    b2World *world;

    b2Body* groundBody;
    b2Body* robotBody;

protected:
    void timerEvent(QTimerEvent *event);

private:
    SimulatorScene *scene;
    int timerId;
};


#endif // SIMULATOR_H
