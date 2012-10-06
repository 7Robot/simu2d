#ifndef OBJECT_H
#define OBJECT_H

#include <string>

struct Object {
public:
    std::string id;
    float x;
    float y;
    float theta;
    float zmin;
    float zmax;

    Object(std::string id, float x, float y, float theta, float zmin, float zmax) :
        id(id), x(x), y(y), theta(theta), zmin(zmin), zmax(zmax)
    {    }
};



#endif // OBJECT_H
