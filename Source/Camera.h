#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>

using namespace std;
using glm::vec3;
using glm::mat3;

class Camera {
public:
  float focalLength;
  float moveSpeed;
  float rotateSpeed;
  float yaw;
  vec3 position;

  vec3 right;
  vec3 down;
  vec3 forward;

  mat3 rotation;


  Camera(float x, float y, float z, float focalLength);
  void translate(vec3 i);
  void rotate(float theta);
};
#endif
