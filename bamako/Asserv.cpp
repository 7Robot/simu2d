#include "math.h"
#include "Asserv.h"

Asserv::Asserv(b2Body *robotBody, b2Vec2 position, float angle) {
    angleSetpoint = 0;
    distanceSetpoint = 0;
    omegaLimit = 1;
    prevPosition = position;
    prevAngle = angle;
    speedLimit = 1;
    motionControl =  DistanceControl; //AngleControl;
    speed = 0;
    speedErrorInt = 0;
    this->robotBody = robotBody;
}


Asserv::~Asserv() {
}

void Asserv::setAngle(float angle) {
    motionControl = AngleControl;
    angleSetpoint = angle+this->prevAngle;
}

void Asserv::setDist(float distance) {
    motionControl = DistanceControl;
    distanceSetpoint = distance;
}

// http://www.iforce2d.net/b2dtut/constant-speed
// http://clubelek.insa-lyon.fr/joomla/fr/base_de_connaissances/informatique/asservissement_et_pilotage_de_robot_auto.php
void Asserv::step(b2Vec2 position, float angle) {
    if(motionControl & DistanceControl) {
        
        // Motion control.
        const float gamma = 0.5; // Acceleration limit (m/s²), cf RCVA diapo 21.

        float speedSetpoint = speed;

        if (fabs(distanceSetpoint) < 0.03)
            speedSetpoint = 0; // ne pas asservir pour de petites distances ...
        else if(fabs(distanceSetpoint) < speedLimit*speedLimit/2/gamma) { // Braking margin.
            speedSetpoint = sqrt(2*gamma*(float)fabs(distanceSetpoint));
            //std::cout << "brake" << std::endl;
        }
        else { // Speed up or plateau.
            speedSetpoint = b2Min(speedLimit, (float)fabs(speed) + B2_TIMESTEP * gamma);
            
            //std::cout << "speed up" << std::endl;
        }
        speedSetpoint *= sgn(distanceSetpoint);
        //std::cout << "motion" << " : " << speed << " - " << speedSetpoint << std::endl;
        // PID
        float speedError = speedSetpoint - speed;
        b2Vec2 speedCmd = speedSetpoint * b2Vec2(cos(prevAngle), sin(prevAngle)); // + speedError * .01 + speedErrorInt * .001;
       
        //std::cout << "cmd: "<< speedCmd.x << " " << speedCmd.y << std::endl;
        speedErrorInt += speedError;
        distanceSetpoint -= (position - prevPosition).Length() * sgn(distanceSetpoint); // Substract covered distance.
        speed = speedSetpoint;
        robotBody->SetLinearVelocity(speedCmd);
    }
    
    if(motionControl & AngleControl) {
        float angleDiff = angleSetpoint-angle;
        //std::cout << angleDiff << std::endl;
        // TODO: faire une pente d'accélération
        if (fabs(angleDiff) > 0.01)
                robotBody->SetAngularVelocity(angleDiff * 3);
    }
    
    prevAngle    = angle;
    prevPosition = position;
}



