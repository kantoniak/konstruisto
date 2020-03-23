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
  float distance = glm::distance(pole_a.get_position().getGlobal(), pole_b.get_position().getGlobal());
  glm::vec2 position = (pole_a.get_position().getGlobal() + pole_b.get_position().getGlobal()) / 2.f;

  float x = pole_a.get_position().getGlobal().x - pole_b.get_position().getGlobal().x;
  float y = pole_a.get_position().getGlobal().y - pole_b.get_position().getGlobal().y;
  float angle = atan2f(x, y);

  glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(position.x, 2.375f, position.y));
  glm::mat4 rotation = glm::rotate(translation, angle, glm::vec3(0, 1, 0));
  glm::mat4 scale = glm::scale(rotation, glm::vec3(1, 1, distance));
  transform = scale;
}

[[nodiscard]] glm::mat4 PowerLineCable::get_transform() const noexcept {
  return transform;
}
}