#include "Collidable.hpp"

namespace geometry {
Collidable::Collidable(layer_key layer, layer_key colliding_layers, Shape::ptr shape, glm::vec2 transform) noexcept
    : layer(layer), colliding_layers(colliding_layers), shape(shape), transform(transform) {
}

bool Collidable::operator==(const Collidable& other) const noexcept {
  return (layer == other.layer) && (shape == other.shape) && (transform == other.transform);
}

const Collidable::layer_key Collidable::get_layer_key() const noexcept {
  return layer;
}

const Collidable::layer_key Collidable::get_colliding_layers() const noexcept {
  return colliding_layers;
}

const Shape& Collidable::get_shape() const noexcept {
  return *shape;
}

const glm::vec2& Collidable::get_transform() const noexcept {
  return transform;
}

bool Collidable::test_collision(const Collidable& other) const noexcept {
  return other.get_shape().test_collision_dd(other.get_transform(), *(this->shape), this->transform);
}

void Collidable::set_user_data(void* user_data) noexcept {
  this->user_data = user_data;
}

void* Collidable::get_user_data() const noexcept {
  return user_data;
}
}