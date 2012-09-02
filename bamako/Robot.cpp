#include "Robot.h"
#include "MainWindow.h"

Robot::Robot(Simulator *simulator, b2Body *robotBody) : robotBody(robotBody), simulator(simulator)
{
    angleSetpoint = 0;
    distanceSetpoint = 0;
    omegaLimit = 1;
    speedLimit = 1;
    motionControl = FreerunControl;

    angle = robotBody->GetAngle();
    position = robotBody->GetPosition();
    speed = 0;

    speedErrorInt = 0;
}

// http://www.iforce2d.net/b2dtut/constant-speed
// http://clubelek.insa-lyon.fr/joomla/fr/base_de_connaissances/informatique/asservissement_et_pilotage_de_robot_auto.php
void Robot::Step()
{
    // Motion control.
    const float gamma = 0.2; // Acceleration limit (m/s²), cf RCVA diapo 21.

    float speedSetpoint = speed;
    if(motionControl & DistanceControl) {
        if(distanceSetpoint < speedLimit*speedLimit/2/gamma) { // Braking margin.
            speedSetpoint = sqrt(2*gamma*distanceSetpoint);
        }
        else { // Speed up or plateau.
            speedSetpoint = b2Min(speedLimit, speed + B2_TIMESTEP * gamma);
        }

        // PID
        float speedError = speedSetpoint - speed;
        float impulse = speedError * .01 + speedErrorInt * .001;
        speedErrorInt += speedError;

        robotBody->ApplyLinearImpulse(impulse * b2Rot(angle).GetXAxis(),
                                      robotBody->GetWorldCenter());
        simulator->mainWindow->plotStep(0, 0);
    }

    if(motionControl & AngleControl && false) {
        float impulse = angleSetpoint * 0.1;
        robotBody->ApplyTorque(impulse);
    }
    //simulator->robotBody->GetLinearVelocityFromLocalPoint()
    //float velChange = desiredVel - vel.x;
    //float impulse = body->GetMass() * velChange; //disregard time factor

    // World simulation.
    simulator->world->Step(B2_TIMESTEP, B2_VELOCITY_ITERATIONS, B2_POSITION_ITERATIONS);

    // Odometry.
    float newAngle = robotBody->GetAngle();
    b2Vec2 newPosition = robotBody->GetPosition();

    distanceSetpoint -= (newPosition - position).Length(); // Substract covered distance.

    angle    = newAngle;
    position = newPosition;
    speed = b2Dot(robotBody->GetLinearVelocity(), b2Rot(angle).GetXAxis());
}

void Robot::KeyboardInput(QMap<int, bool> keyStates)
{
    distanceSetpoint = keyStates['Z'] - keyStates['S'];
    angleSetpoint    = keyStates['D'] - keyStates['Q'];
}
