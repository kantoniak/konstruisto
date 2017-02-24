#ifndef DATA_CAMERASTATE_HPP
#define DATA_CAMERASTATE_HPP

#include <glm/glm.hpp>

namespace data {
struct CameraState {
  glm::vec3 lookAt;
  float rotationAroundX, rotationAroundY;
  float distance;
};
}

#endif