#ifndef ROBOT_H
#define ROBOT_H

#include <Box2D.h>
#include "Asserv.h"
#include "Simulator.h"


class Robot
{
public:
    Robot(Simulator *simulator, b2Body *robotBody);
    void Step();
    void KeyboardInput(QMap<int, bool> keyStates);

    float angle;
    Asserv *asserv;
    b2Vec2 position;
    b2Body *robotBody;

private:
    Simulator *simulator;

    // Les consignes arriveront dans le signal QTcpSocket::readyRead().
};

#endif // ROBOT_H
