#ifndef LIGHTING_H
#define LIGHTING_H

#include <glm/glm.hpp>
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "Intersection.h"


using namespace std;
using glm::vec3;
using glm::mat3;

class Lighting {
public:
  vec3 lightPos;
  vec3 lightColor;
  std::default_random_engine generator;
  std::uniform_real_distribution<float> distribution;


  Lighting(vec3 lightPos, vec3 lightColor);
  vec3 DirectLight(const Intersection& i , const vector<Triangle>& triangles);
  vec3 IndirectLight(const Intersection& i , const vector<Triangle>& triangles, int limit, int sample);
  vec3 CombineColor(const Intersection& i,const vector<Triangle>& triangles, int limit, int sample);
  vec3 PointOnHemisphere(const vec3 N);
};

#endif
