#ifndef DATA_CAMERASTATE_HPP
#define DATA_CAMERASTATE_HPP

#include <ostream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

namespace data {
struct CameraState {
  glm::vec3 lookAt;
  float rotationAroundX, rotationAroundY;
  float distance;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(lookAt);
    archive(rotationAroundX, rotationAroundY);
    archive(distance);
  }

  friend std::ostream& operator<<(std::ostream& os, const CameraState& camera_state) noexcept;
  friend std::string to_string(const CameraState& camear_state) noexcept;
};
}

#endif