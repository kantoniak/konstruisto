#include "Brush.hpp"

namespace input {

Brush::Brush(data::Position<float> center, float radius, float border_width) noexcept
    : center(center), radius(radius), border_width(border_width) {
}

void Brush::set_active(bool active) noexcept {
  this->active = active;
}

bool Brush::is_active() const noexcept {
  return this->active;
}

void Brush::set_center(const data::Position<float>& center) noexcept {
  this->center = center;
}

const data::Position<float>& Brush::get_center() const noexcept {
  return this->center;
}

void Brush::set_radius(float radius) noexcept {
  this->radius = radius;
}

float Brush::get_radius() const noexcept {
  return this->radius;
}

float Brush::get_border_width() const noexcept {
  return this->border_width;
}

void Brush::set_base_colors(glm::vec4 fill, glm::vec4 border) noexcept {
  this->fill_color_base = fill;
  this->border_color_base = border;
}

void Brush::set_active_colors(glm::vec4 fill, glm::vec4 border) noexcept {
  this->fill_color_active = fill;
  this->border_color_active = border;
}

glm::vec4 Brush::get_fill_color_base() const noexcept {
  return this->fill_color_base;
}

glm::vec4 Brush::get_fill_color_active() const noexcept {
  return this->fill_color_active;
}

glm::vec4 Brush::get_border_color_base() const noexcept {
  return this->border_color_base;
}

glm::vec4 Brush::get_border_color_active() const noexcept {
  return this->border_color_active;
}

}