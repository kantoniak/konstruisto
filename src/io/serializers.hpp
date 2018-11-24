#ifndef IO_SERIALIZERS_HPP
#define IO_SERIALIZERS_HPP

#include <glm/glm.hpp>

namespace glm {
template <typename Archive>
void serialize(Archive& archive, glm::ivec2& v) {
  archive(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y));
}

template <typename Archive>
void serialize(Archive& archive, glm::vec3& v) {
  archive(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y), cereal::make_nvp("z", v.z));
}
}

#endif
