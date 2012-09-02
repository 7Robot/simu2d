#ifndef ROBOT_H
#define ROBOT_H

#include <Box2D.h>
#include "Simulator.h"

enum MotionControlType
{
    FreerunControl  = 0,
    AngleControl    = 0x1,
    DistanceControl = 0x2,
    PolarControl    = 0x3,
    SpeedControl    = 0x4
};

class Robot
{
public:
    Robot(Simulator *simulator, b2Body *robotBody);
    void Step();
    void KeyboardInput(QMap<int, bool> keyStates);

    float angle;
    b2Vec2 position;
    float speed;
    b2Body *robotBody;

private:
    float angleSetpoint;
    float distanceSetpoint;
    float omegaLimit;
    float speedLimit;
    MotionControlType motionControl;

    // PID
    float speedErrorInt;

    Simulator *simulator;

    // Les consignes arriveront dans le signal QTcpSocket::readyRead().
};

#endif // ROBOT_H
