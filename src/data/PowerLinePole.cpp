#include "PowerLinePole.hpp"

namespace data {

PowerLinePole::PowerLinePole(Position<float> position) noexcept
    : position(position), cable_snapping_points({glm::vec3(-0.6f, 2.0f, 0.0f), glm::vec3(-0.6f, 1.5f, 0.0f),
                                                 glm::vec3(0.6f, 1.5f, 0.0f), glm::vec3(0.6f, 2.0f, 0.0f)}),
      snapping_points_count(cable_snapping_points.size()) {
  transformed_cable_snapping_points.resize(snapping_points_count);
  init_matrices();
}

bool PowerLinePole::operator==(const PowerLinePole& other) const noexcept {
  return other.position == position;
}

bool PowerLinePole::operator!=(const PowerLinePole& other) const noexcept {
  return !(other.position == position);
}

Position<float> PowerLinePole::get_position() const noexcept {
  return position;
}

glm::mat4 PowerLinePole::get_transform() const noexcept {
  return transform;
}

void PowerLinePole::set_translation(Position<float> position) noexcept {
  this->position = position;
  update_transform();
}

void PowerLinePole::set_rotation(float rotation) noexcept {
  this->rotation_angle = rotation;
  this->rotation = glm::rotate(glm::mat4(1), rotation_angle, glm::vec3(0, 1, 0));
  update_transform();
}

float PowerLinePole::get_rotation() const noexcept {
  return this->rotation_angle;
}

size_t PowerLinePole::get_cable_snapping_points_count() const noexcept {
  return snapping_points_count;
}

const std::vector<glm::vec3>& PowerLinePole::get_cable_snapping_points() const noexcept {
  return transformed_cable_snapping_points;
}

bool PowerLinePole::add_neighbor(PowerLinePole::ptr neighbor) noexcept {
  return neighbors.insert(neighbor).second;
}

const std::set<PowerLinePole::ptr>& PowerLinePole::get_neigbors() const noexcept {
  return neighbors;
}

void PowerLinePole::init_matrices() noexcept {
  this->rotation_angle = 0;
  this->rotation = glm::mat4(1.0f);
  update_transform();
}

void PowerLinePole::update_transform() noexcept {
  translation = glm::translate(glm::mat4(1), glm::vec3(position.getGlobal().x, 0, position.getGlobal().y));
  transform = translation * rotation;

  for (size_t i = 0; i < snapping_points_count; i++) {
    transformed_cable_snapping_points[i] = transform * glm::vec4(cable_snapping_points[i], 1);
  }
}
}