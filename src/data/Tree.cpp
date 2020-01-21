#include "Tree.hpp"

#include <utility>

namespace data {

Tree::Tree(Type type, Position<float> position, float rotation, float age, geometry::Collidable::ptr body) noexcept
    : type(type), position(position), rotation_angle(rotation), age(age), body(std::move(body)) {
  init_matrices();
}

bool Tree::operator==(const Tree& other) const noexcept {
  return other.position == position && other.age == age && other.rotation_angle == rotation_angle;
}

void Tree::add_age(float age_delta) noexcept {
  age += 10.f * age_delta;
  update_transform();
}

Tree::Type Tree::get_type() const noexcept {
  return type;
}

Position<float> Tree::get_position() const noexcept {
  return position;
}

glm::mat4 Tree::get_transform() const noexcept {
  return transform;
}

geometry::Collidable::ptr Tree::get_body() const noexcept {
  return body;
}

void Tree::init_matrices() noexcept {
  translation = glm::translate(glm::mat4(1), glm::vec3(position.getGlobal().x, 0, position.getGlobal().y));
  rotation = glm::rotate(glm::mat4(1), rotation_angle, glm::vec3(0, 1, 0));
  update_transform();
}

void Tree::update_transform() noexcept {
  // Scale calculation
  float g = (age + 40.f) / 70000.f;
  float h = sqrt(sqrtf(1.f / g));
  float hh = sqrt(sqrtf(1.f / h));
  float f = 0.5f + (6 - h - hh) / 7.f;

  float scaleFactor;
  if (type == MODEL2) {
    scaleFactor = 1.f;
  } else {
    scaleFactor = 1.6f;
  }

  transform = translation * glm::scale(rotation, glm::vec3(f, f * scaleFactor, f));
}
}