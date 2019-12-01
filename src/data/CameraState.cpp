#include "CameraState.hpp"

namespace data {

std::ostream& operator<<(std::ostream& os, const CameraState& camera_state) noexcept {
  return os << "CameraState [lookAt=" << glm::to_string(camera_state.lookAt)
            << ", rotationAroundX=" << camera_state.rotationAroundX
            << ", rotationAroundY=" << camera_state.rotationAroundY << ", dist=" << camera_state.distance << "]";
}

std::string to_string(const CameraState& camera_state) noexcept {
  std::stringstream result;
  result << camera_state;
  return result.str();
}
}