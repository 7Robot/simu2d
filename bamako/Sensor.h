
#ifndef SENSOR_H
#define	SENSOR_H

#include <stdio.h>

#include "Box2D/Box2D.h"
#include "common.h"
#include "Robot.h"

class RayCastCallback : public b2RayCastCallback
{
public:
    RayCastCallback()
    {       
        m_hit = false;
        m_fixture = NULL;
        body = NULL;
    }
float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,
        const b2Vec2& normal, float32 fraction)
    {
        m_fixture = fixture;
        body = fixture->GetBody();
        m_point = point;
        m_normal = normal;
        m_fraction = fraction;
        m_hit = true;
        return fraction;
    }
bool m_hit;
b2Body* body;
b2Fixture* m_fixture;
b2Vec2 m_point;
b2Vec2 m_normal;
float32 m_fraction;
};

class Robot; // FIXME : circular references -> patron observateur

class Sensor {
public:
    Sensor(b2World* world, Robot* robot, b2Vec2 sensorPos, float height, 
            float sensorTheta, float rangeMax);
    // TODO: bruit gaussien sur les mesures
    virtual ~Sensor();
    virtual float sense();
    
    float    sensorTheta;
    bool mute;
    
protected:
    Robot*   robot;
    b2World* world;
    b2Vec2   sensorPos;
    float    height;
    float    rangeMax;
private:
    DISALLOW_COPY_AND_ASSIGN(Sensor);

};

class Bumper : public Sensor {
public:
    Bumper(b2World* world, Robot* robot, b2Vec2 sensorPos, float height, 
            float sensorTheta);
    virtual ~Bumper();
    float sense();
    
private:
    DISALLOW_COPY_AND_ASSIGN(Bumper);
};

enum RFMode {
    broadcast,
    range
};


class RangeFinder : public Sensor {
public:
    RangeFinder(b2World* world, Robot* robot, b2Vec2 sensorPos, float height, 
            float sensorTheta, float rangeMax);
    virtual ~RangeFinder();
    float sense();
    
    RFMode mode;
    float  threshold;
    
private:
    float prevDist;
    DISALLOW_COPY_AND_ASSIGN(RangeFinder);
};

class Turret180 : public Sensor {
public:
    Turret180(b2World* world, Robot* robot, b2Vec2 sensorPos, float height, 
            float sensorTheta, float rangeMax);
    virtual ~Turret180();
    float sense();
    
private:
    float angle;
    int rotSide;
    RangeFinder* sensor;
    DISALLOW_COPY_AND_ASSIGN(Turret180);
};

#endif	/* SENSOR_H */

