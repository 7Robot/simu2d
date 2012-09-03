#ifndef ASSERV_H
#define	ASSERV_H

/* On peut vouloir changer de techniques d'asservissement 
 * sans changer le code du robot.
 * Donc nécessité d'une interface et de classes d'asservissement
 * Ici une asserv distance/oriantation (non testée !)
 */

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

class Asserv {
    
public:
    Asserv(b2Body *robotBody, b2Vec2 position, float angle);
    virtual ~Asserv();
    
    void setAngle(float angle);
    void setDist(float distance);
    void step(b2Vec2 position, float angle);
    
private:
    b2Body *robotBody;
    float angleSetpoint;
    float distanceSetpoint;
    float omegaLimit;
    float speedLimit;
    MotionControlType motionControl;
    float speed;
    b2Vec2 prevPosition;
    float prevAngle;
    

    // PID
    float speedErrorInt;

};

#endif	/* ASSERV_H */

