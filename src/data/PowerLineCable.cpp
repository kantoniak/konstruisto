#include "PowerLineCable.hpp"

namespace data {

PowerLineCable::PowerLineCable(const PowerLinePole& pole_a, const PowerLinePole& pole_b) noexcept
    : pole_a(pole_a), pole_b(pole_b) {
  update_transform();
}

bool PowerLineCable::operator==(const PowerLineCable& other) const noexcept {
  return pole_a == other.pole_a && pole_b == other.pole_b;
}

void PowerLineCable::update_transform() noexcept {
  glm::vec2 position = (pole_a.get_position().getGlobal() + pole_b.get_position().getGlobal()) / 2.f;
  transform = glm::translate(glm::mat4(1.f), glm::vec3(position.x, 2.375f, position.y));
}

[[nodiscard]] glm::mat4 PowerLineCable::get_transform() const noexcept {
  return transform;
}
}