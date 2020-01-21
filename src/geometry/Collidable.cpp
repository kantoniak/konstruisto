#include "Collidable.hpp"

namespace geometry {
Collidable::Collidable(std::shared_ptr<Shape> shape, glm::vec2 transform) noexcept
    : shape(shape), transform(transform) {
}

bool Collidable::operator==(const Collidable& other) const noexcept {
  return (shape == other.shape) && (transform == other.transform);
}

const std::shared_ptr<Shape>& Collidable::get_shape() const noexcept {
  return shape;
}

const glm::vec2& Collidable::get_transform() const noexcept {
  return transform;
}

bool Collidable::test_collision(const Collidable& other) const noexcept {
  return other.get_shape()->test_collision_dd(other.get_transform(), *(this->shape), this->transform);
}
}