#include "Asserv.h"

Asserv::Asserv(b2Body *robotBody, b2Vec2 position, float angle) {
    angleSetpoint = 0;
    distanceSetpoint = 0;
    omegaLimit = 1;
    prevPosition = position;
    prevAngle = angle;
    speedLimit = 1;
    motionControl = FreerunControl;
    speed = 0;
    speedErrorInt = 0;
    this->robotBody = robotBody;
}


Asserv::~Asserv() {
}

void Asserv::setAngle(float angle) {
    angleSetpoint = angle;
}

void Asserv::setDist(float distance) {
    distanceSetpoint = distance;
}

// http://www.iforce2d.net/b2dtut/constant-speed
// http://clubelek.insa-lyon.fr/joomla/fr/base_de_connaissances/informatique/asservissement_et_pilotage_de_robot_auto.php
void Asserv::step(b2Vec2 position, float angle) {
    if(motionControl & DistanceControl) {
        // Motion control.
        const float gamma = 0.2; // Acceleration limit (m/s²), cf RCVA diapo 21.

        float speedSetpoint = speed;

        if(distanceSetpoint < speedLimit*speedLimit/2/gamma) { // Braking margin.
            speedSetpoint = sqrt(2*gamma*distanceSetpoint);
        }
        else { // Speed up or plateau.
            speedSetpoint = b2Min(speedLimit, speed + B2_TIMESTEP * gamma);
        }

        // PID
        float speedError = speedSetpoint - speed;
        b2Vec2 speedCmd = b2Vec2(0, 0);
        float speed = robotBody->GetLinearVelocity().Normalize();
        if (speed > 0)
                speedCmd = speedSetpoint * 1/speed * robotBody->GetLinearVelocity(); // + speedError * .01 + speedErrorInt * .001;
        
        speedErrorInt += speedError;
        distanceSetpoint -= (position - prevPosition).Length(); // Substract covered distance.
        
        robotBody->SetLinearVelocity(speedCmd);
    }
    
    if(motionControl & AngleControl) {
        float angleDiff = angleSetpoint-angle;
        if (angleDiff < -b2_pi)
            angleDiff += 2 * b2_pi;
        else if (angleDiff > b2_pi)
            angleDiff -= 2 * b2_pi; 
        int sign = 1;
        if (angleDiff < 0)
            sign = -1;
        robotBody->SetAngularVelocity(sign * angleDiff * 0.1);
    }
    
    prevAngle    = angle;
    prevPosition = position;
}



