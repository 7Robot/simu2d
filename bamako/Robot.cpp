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
    distanceControl  = ControlSetpoint;

    angleSetpoint = 0; // rad
    angleSetSpeed = 0; // rad.s⁻¹
    angleMaxSpeed = 3; // rad.s⁻¹ FIXME
    angleMaxAccel = 6; // rad.s⁻² FIXME
    angleControl  = ControlSetpoint;
}

// http://www.iforce2d.net/b2dtut/constant-speed
// http://clubelek.insa-lyon.fr/joomla/fr/base_de_connaissances/informatique/asservissement_et_pilotage_de_robot_auto.php
void Robot::Step()
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
        static float error = 0, errorInteg = 0;
        float newError = distanceSetSpeed - speed;
        float errorDeriv = fabs(newError - error); // fabs ?
        error = newError;

        errorInteg += error;
        errorInteg = b2Clamp(errorInteg, -1.f, 1.f);

        float impulse = error * .1 + errorInteg * .1 + errorDeriv * .00;

        robotBody->ApplyLinearImpulse(impulse * b2Rot(angle).GetXAxis(),
                                      robotBody->GetWorldCenter());
        simulator->plotStep(impulse, speed);
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
        static float error = 0, errorInteg = 0;
        float newError = angleSetSpeed - angularSpeed;
        float errorDeriv = fabs(newError - error);
        error = newError;

        errorInteg += error;

        float impulse = error * .4 + errorInteg * 0.2 - errorDeriv * .05;

        robotBody->ApplyTorque(impulse);
    }

    // FIXME résister aux déplacements pas dans le sens des roues.
    //simulator->robotBody->GetLinearVelocityFromLocalPoint()
    //float impulse = body->GetMass() * velChange; //disregard time factor

    // World simulation.
    simulator->world->Step(B2_TIMESTEP, B2_VELOCITY_ITERATIONS, B2_POSITION_ITERATIONS);

    // Substract covered distance.
    b2Vec2 newPosition = robotBody->GetPosition();
    float  newAngle    = robotBody->GetAngle();

    distanceSetpoint -= b2Dot(newPosition - position, b2Rot((newAngle + angle)/2).GetXAxis());
    angleSetpoint -= newAngle - angle;

    position = newPosition;
    angle    = newAngle;
}

void Robot::KeyboardInput(QMap<int, bool> keyStates)
{
    distanceControl  = ControlSpeed;
    distanceSetSpeed = (keyStates['Z'] - keyStates['S']) * distanceMaxSpeed;
    angleControl     = ControlSpeed;
    angleSetSpeed    = (keyStates['D'] - keyStates['Q']) * angleMaxSpeed;
}
