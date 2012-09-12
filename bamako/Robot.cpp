#include "Robot.h"
#include "MainWindow.h"

Robot::Robot(Simulator *simulator, b2Body *robotBody) : robotBody(robotBody), simulator(simulator)
{
    position = robotBody->GetPosition();
    angle    = robotBody->GetAngle();
    positionOffset.SetZero();
    angleOffset    = 0;

    distanceSetpoint = 0; // m
    distanceSetSpeed = 0; // m.s⁻¹
    distanceMaxSpeed = 1; // m.s⁻¹
    distanceMaxAccel = 2; // m.s⁻² Diapo 21.
    distanceError = 0;
    distanceErrorInteg = 0;
    distanceControl  = ControlSetpoint;

    angleSetpoint = 0; // rad
    angleSetSpeed = 0; // rad.s⁻¹
    angleMaxSpeed = 3; // rad.s⁻¹ FIXME
    angleMaxAccel = 6; // rad.s⁻² FIXME
    angleError = 0;
    angleErrorInteg = 0;
    angleControl  = ControlSetpoint;
}

// http://www.iforce2d.net/b2dtut/constant-speed
// http://clubelek.insa-lyon.fr/joomla/fr/base_de_connaissances/informatique/asservissement_et_pilotage_de_robot_auto.php
void Robot::preStep()
{
    float speed = b2Dot(robotBody->GetLinearVelocity(), b2Rot(angle).GetXAxis());
    float angularSpeed = robotBody->GetAngularVelocity();

    // Motion control for distances.
    if(distanceControl != ControlFreerun) {
        if(distanceControl == ControlSetpoint) { // Compute the desired speed.
            if(fabs(distanceSetpoint) < distanceMaxSpeed * distanceMaxSpeed / 2 / distanceMaxAccel) { // Braking margin.
                distanceSetSpeed = sqrt(fabs(distanceSetpoint * 2 * distanceMaxAccel)); // TODO il devrait y avoir la vitesse actuelle
            }
            else { // Speed up or plateau.
                distanceSetSpeed = b2Min(distanceMaxSpeed,
                                         (float)fabs(speed) + B2_TIMESTEP * distanceMaxAccel);
            }
            distanceSetSpeed *= sgn(distanceSetpoint);
        }


        // PID.
        float newError = distanceSetSpeed - speed;
        float errorDeriv = fabs(newError - distanceError); // fabs ?
        distanceError = newError;

        distanceErrorInteg += distanceError;
        distanceErrorInteg = b2Clamp(distanceErrorInteg, -1.f, 1.f);

        float impulse = distanceError * .1 + distanceErrorInteg * .1 + errorDeriv * .0;

        robotBody->ApplyLinearImpulse(impulse * b2Rot(angle).GetXAxis(),
                                      robotBody->GetWorldCenter());
        if(this == simulator->robot)
            simulator->mainWindow->debugPlot(impulse, speed);
    }

    // Motion control for angles.
    if(angleControl != ControlFreerun) {
        if(angleControl == ControlSetpoint) { // Compute the desired speed.
            if(fabs(angleSetpoint) < angleMaxSpeed * angleMaxSpeed / 2 / angleMaxAccel) { // Braking margin.
                angleSetSpeed = sqrt(fabs(angleSetpoint * 2 * angleMaxAccel));
            }
            else { // Speed up or plateau.
                angleSetSpeed = b2Min(angleMaxSpeed,
                                     (float)fabs(angularSpeed) + B2_TIMESTEP * angleMaxAccel);
            }
            angleSetSpeed *= sgn(angleSetpoint);
        }


        // PID.
        float newError = angleSetSpeed - angularSpeed;
        float errorDeriv = fabs(newError - angleError);
        angleError = newError;

        angleErrorInteg += angleError;

        float impulse = angleError * .4 + angleErrorInteg * 0.2 - errorDeriv * .05;

        robotBody->ApplyTorque(impulse);
    }

    // FIXME résister aux déplacements pas dans le sens des roues.
    //simulator->robotBody->GetLinearVelocityFromLocalPoint()
    //float impulse = body->GetMass() * velChange; //disregard time factor
}

void Robot::postStep()
{
    // Substract covered distance.
    b2Vec2 newPosition = robotBody->GetPosition();
    float  newAngle    = robotBody->GetAngle();

    distanceSetpoint -= b2Dot(newPosition - position, b2Rot((newAngle + angle)/2).GetXAxis());
    angleSetpoint -= newAngle - angle;

    position = newPosition;
    angle    = newAngle;
}

void Robot::keyboardInput(QMap<int, bool> keyStates)
{
    distanceControl  = ControlSpeed;
    distanceSetSpeed = (keyStates['Z'] - keyStates['S']) * distanceMaxSpeed;
    angleControl     = ControlSpeed;
    angleSetSpeed    = (keyStates['D'] - keyStates['Q']) * angleMaxSpeed;
}
