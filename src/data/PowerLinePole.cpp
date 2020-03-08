#include "PowerLinePole.hpp"

namespace data {

PowerLinePole::PowerLinePole(Position<float> position) noexcept : position(position) {
  init_matrices();
}

bool PowerLinePole::operator==(const PowerLinePole& other) const noexcept {
  return other.position == position;
}

Position<float> PowerLinePole::get_position() const noexcept {
  return position;
}

glm::mat4 PowerLinePole::get_transform() const noexcept {
  return transform;
}

void PowerLinePole::set_rotation(float rotation) noexcept {
  this->rotation = glm::rotate(glm::mat4(1), rotation, glm::vec3(0, 1, 0));
  update_transform();
}

void PowerLinePole::init_matrices() noexcept {
  translation = glm::translate(glm::mat4(1), glm::vec3(position.getGlobal().x, 0, position.getGlobal().y));
  rotation = glm::mat4(1.0f);
  update_transform();
}

void PowerLinePole::update_transform() noexcept {
  transform = translation * rotation;
}
}