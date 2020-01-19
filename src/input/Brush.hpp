#ifndef INPUT_BRUSH_HPP
#define INPUT_BRUSH_HPP

#include <glm/glm.hpp>

#include "../data/Position.hpp"

namespace input {

class Brush {

public:
  Brush(data::Position<float> center, float radius, float border_width) noexcept;

  void set_active(bool active) noexcept;
  [[nodiscard]] bool is_active() const noexcept;

  void set_center(const data::Position<float>&) noexcept;
  [[nodiscard]] const data::Position<float>& get_center() const noexcept;

  void set_radius(float radius) noexcept;
  [[nodiscard]] float get_radius() const noexcept;
  [[nodiscard]] float get_border_width() const noexcept;

  void set_base_colors(glm::vec4 fill, glm::vec4 border) noexcept;
  void set_active_colors(glm::vec4 fill, glm::vec4 border) noexcept;
  [[nodiscard]] glm::vec4 get_fill_color_base() const noexcept;
  [[nodiscard]] glm::vec4 get_fill_color_active() const noexcept;
  [[nodiscard]] glm::vec4 get_border_color_base() const noexcept;
  [[nodiscard]] glm::vec4 get_border_color_active() const noexcept;

protected:
  bool active = false;
  data::Position<float> center;
  float radius;
  float border_width;
  glm::vec4 fill_color_base;
  glm::vec4 fill_color_active;
  glm::vec4 border_color_base;
  glm::vec4 border_color_active;
};
}

#endif
