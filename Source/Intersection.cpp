#include <glm/glm.hpp>
#include "Intersection.h"
/*the implementation of this class is followed the guidance from the pdf*/
bool Intersection::ClosestIntersection(vec3 start, vec3 dir, const vector<Triangle>& triangles, Intersection& closestIntersection, int ignore)
{

    float min = std::numeric_limits<float>::max();

    for( unsigned int i = 0; i < triangles.size(); i++){
        if ((int) i == ignore) {
            continue;
        }

        vec3 v0 = triangles[i].v0;
        vec3 v1 = triangles[i].v1;
        vec3 v2 = triangles[i].v2;

        vec3 e1 = v1 - v0;
        vec3 e2 = v2 - v0;
        vec3 b = start - v0;

        mat3 A( -dir, e1, e2 );
        vec3 x = glm::inverse( A ) * b;
        //for the point r on the surface, r =v0 + ue1 + ve2,  we say that u>0 && v>0 && u+v<=1
        if(x[0] > 0 && x[1]>=0 && x[2]>=0 && (x[1]+x[2]) <= 1){
            if(x[0] <= min){
                min = x.x;
                closestIntersection.distance = min;
                closestIntersection.position = start + x[0] * dir;//r = s + td
                closestIntersection.triangleIndex = i;
            }
        }
    }
    if(min<std::numeric_limits<float>::max()){
        return true;
    }
    return false;
}
