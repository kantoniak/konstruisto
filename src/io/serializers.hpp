#ifndef IO_SERIALIZERS_HPP
#define IO_SERIALIZERS_HPP

#include <glm/glm.hpp>

namespace glm {
template <typename Archive> void serialize(Archive& archive, glm::vec3& v3) {
  archive(cereal::make_nvp("x", v3.x), cereal::make_nvp("y", v3.y), cereal::make_nvp("z", v3.z));
}
}

#endif
