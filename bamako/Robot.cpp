#include "Robot.h"
#include "MainWindow.h"


Robot::Robot(Simulator *simulator, b2Body *robotBody) : robotBody(robotBody), simulator(simulator)
{
    angle = robotBody->GetAngle();
    position = robotBody->GetPosition();
    asserv = new Asserv(robotBody, position, angle);
}


void Robot::Step()
{
    // Odometry.
    angle = robotBody->GetAngle();
    position = robotBody->GetPosition();
    
    // Asserv
    asserv->step(position, angle);   

    // World simulation.
    simulator->world->Step(B2_TIMESTEP, B2_VELOCITY_ITERATIONS, B2_POSITION_ITERATIONS);
    
}

void Robot::KeyboardInput(QMap<int, bool> keyStates)
{
    //distanceSetpoint = keyStates['Z'] - keyStates['S'];
    //angleSetpoint    = keyStates['D'] - keyStates['Q'];
}
