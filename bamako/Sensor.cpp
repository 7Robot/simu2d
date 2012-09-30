
#include "Sensor.h"
#include "Robot.h"

#include<math.h>

Sensor::Sensor(b2World* world, Robot* robot, b2Vec2 sensorPos, float height, 
            float sensorTheta, float rangeMax) : world(world), 
        robot(robot), rangeMax(rangeMax), sensorPos(sensorPos), height(height),
        sensorTheta(sensorTheta), mute(false)  { 
}


Sensor::~Sensor() {
}

/**
 * Mesure la distance de l'objet le plus près
 * @return la distance ou 0 si rien n'est mesuré
 */
// FIXME : prende en compte la hauteur des objets
float Sensor::sense()
{
    if (mute)
        return 0;
    else
    {
        RayCastCallback callback;
        world->RayCast(&callback, robot->position+sensorPos, 
                b2Vec2(rangeMax*cosf(robot->angle+sensorTheta)+robot->position.x+sensorPos.x, 
                       rangeMax*sinf(robot->angle+sensorTheta)+robot->position.y+sensorPos.y));


        if (callback.m_hit)
        {
            return (callback.m_point - robot->position).Length();
        }
        else
        {
            return 0;
        }
    }
}

Bumper::Bumper(b2World* world, Robot* robot, b2Vec2 sensorPos, float height, 
            float sensorTheta) : 
        Sensor(world, robot, sensorPos, height, sensorTheta, 0.04)  {
}

Bumper::~Bumper() {
}

Bumper::sense()
{
    return Sensor::sense();
}

RangeFinder::RangeFinder(b2World* world, Robot* robot, b2Vec2 sensorPos, float height, 
            float sensorTheta, float rangeMax) : 
        Sensor(world, robot, sensorPos, height, sensorTheta, rangeMax)  {
    mode = broadcast;
    threshold = 1;
    prevDist = rangeMax + 1;
}


RangeFinder::~RangeFinder() {
}

float RangeFinder::sense()
{
    if (mode==broadcast)
        return Sensor::sense();
    else
    {
        float dist = Sensor::sense();
        if (dist==0)
            return 0;
        else if (dist < threshold && prevDist > threshold)
        {
            threshold += 0.02; // hystéresis
            prevDist = dist;
            return dist;
        }
        else if (dist > threshold &&  prevDist < threshold)
        {
            threshold -= 0.02; // hystéresis
            prevDist = dist;
            return dist;
        }
        else
        {
            prevDist = dist;
            return 0;
        }
    }
}