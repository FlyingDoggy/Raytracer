#include <glm/glm.hpp>
#include <iostream>
#include <cmath>
#include "Lighting.h"

using namespace std;
using glm::vec3;
using glm::mat3;


Lighting::Lighting(vec3 lightPos, vec3 lightColor):lightPos(lightPos),lightColor(lightColor){


}
/*this function is used to find the direction of the indirect light*/
vec3 Lighting::PointOnHemisphere(const vec3 N) {
    vec3 newDir(0,0,0);
    float angle;
    do{ newDir = vec3(0,1,0);//a vector on y-axis
        float u = distribution(generator)*2*M_PI;
        float v = distribution(generator)*2*M_PI;
        newDir = glm::rotateZ(newDir,u);//first rotate on z axis
        newDir = glm::rotateY(newDir,v);//then totate on y axis
        angle = glm::angle(newDir, N);//calcluate the angle between normal and new direction
    }while(angle >= M_PI/2);//if the angle is larger than 90 degree, it means the new point is not on the hemishpere

    return newDir;
}
/*this function calcuate the indirect light of pixels*/
vec3 Lighting::IndirectLight(const Intersection& i,const vector<Triangle>& triangles, int limit, int sample){
    Intersection intersect;
    vec3 colour(0,0,0);
    for(int count = 0; count < sample; count++){
        vec3 dir = PointOnHemisphere(triangles[i.triangleIndex].normal);
        //if there are Intersection along the new direction, the light from that Intersection has been recorded
        if(Intersection::ClosestIntersection(i.position,dir,triangles,intersect, i.triangleIndex)){
            colour+= CombineColor(intersect, triangles, limit-1, sample);
        }
    }
    return colour/(float)sample;
}
/*this function calcuate the direct light of each pixel
it also claculate the shadow by calculate the difference between distance(start to light position) and distance(start to intersection) */
vec3 Lighting::DirectLight( const Intersection& i,const vector<Triangle>& triangles ){
    Intersection intersect;
    vec3 powerD;

    vec3 r = lightPos-i.position;//unit vector r describing the direction from the surface point to the light source

    bool found = Intersection::ClosestIntersection(i.position, r, triangles, intersect, i.triangleIndex) ;

    if (found && glm::length(r) > glm::length(intersect.position - i.position)) {
        powerD = vec3(0,0,0);
    } else {
        vec3 n = triangles[i.triangleIndex].normal;//unit vector n describing the normal pointing out from the surface

        float disFromLight = glm::length(r);
        float dotProduct = glm::dot(r, n);

        vec3 powerB = lightColor / (float)(4*M_PI*disFromLight*disFromLight);

        if(dotProduct >= 0){
            powerD = powerB*dotProduct;

        }else{
            powerD = vec3(0,0,0);
        }
    }

    return powerD;

}
/*this function combine the direct light and indirect light together and calcualte the pixel value*/
vec3 Lighting::CombineColor(const Intersection& i,const vector<Triangle>& triangles, int limit, int sample){
    vec3 direct = DirectLight(i, triangles);
    vec3 indirect;
    //limit is the number of the bounce of the light
    if (limit > 0) {//while still bouncing,calcuate the indirect light recursively 
        indirect = IndirectLight(i, triangles, limit, sample);
    } else {
        indirect = vec3(0, 0, 0);
    }

    vec3 finalColor = triangles[i.triangleIndex].color * (direct+indirect);
    return finalColor;
}
