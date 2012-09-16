#ifndef ROBOT_H
#define ROBOT_H

#include <Box2D/Box2D.h>
#include "Simulator.h"

enum MotionControl
{
    ControlFreerun,
    ControlSetpoint,
    ControlSpeed
};

class Robot
{
public:
    Robot(Simulator *simulator, b2Body *robotBody);
    void preStep();
    void postStep();
    void keyboardInput(QMap<int, bool> keyStates);

    b2Body *robotBody;

    // Motion control.
    float distanceSetpoint;
    float distanceSetSpeed;
    float distanceMaxSpeed;
    float distanceMaxAccel;
    float distanceError;
    float distanceErrorInteg;
    MotionControl distanceControl;
    float angleSetpoint;
    float angleSetSpeed;
    float angleMaxSpeed;
    float angleMaxAccel;
    float angleError;
    float angleErrorInteg;
    MotionControl angleControl;

    // Odometry.
    b2Vec2 position;
    b2Vec2 positionOffset;
    float angle;
    float angleOffset;


private:
    Simulator *simulator;
};


template <typename T> int sgn(T val) // http://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
{
    return (T(0) < val) - (val < T(0));
}

#endif // ROBOT_H
