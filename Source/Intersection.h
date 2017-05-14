#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <glm/glm.hpp>
#include <vector>
#include "Triangle.h"

using namespace std;
using glm::vec3;
using glm::mat3;

class Intersection{
public:
    vec3 position;
    float distance;
    int triangleIndex;
    //this function is used to determine if the found intersection is the closest one, ignore parameter is used to ignore the surface itself
    static bool ClosestIntersection(vec3 start, vec3 dir, const vector<Triangle>& triangles, Intersection& closestIntersection, int ignore);
};
#endif
