#include <glm/glm.hpp>
#include "Camera.h"

using namespace std;
using glm::vec3;
using glm::mat3;
/*set the parameters*/
Camera::Camera(float x, float y, float z, float focalLength): focalLength(focalLength) {
  position.x = x;
  position.y = y;
  position.z = z;
  moveSpeed = 1;
  rotateSpeed = 0.5;
  rotation = mat3(1.0);
}
/*translation function*/
void Camera::translate(vec3 i){
  position += rotation*i;
}
/*rotation function*/
void Camera::rotate(float theta){
  yaw += theta;
  //calculate the rotation matrix
  rotation[0][0] = cos(yaw);
  rotation[0][2] = -sin(yaw);
  rotation[2][0] = sin(yaw);
  rotation[2][2] = cos(yaw);

}
