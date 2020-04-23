#include "PowerLineCable.hpp"

namespace data {

PowerLineCable::PowerLineCable(const PowerLinePole& pole_a, const PowerLinePole& pole_b) noexcept
    : pole_a(pole_a), pole_b(pole_b), cable_count(pole_a.get_cable_snapping_points_count()) {
  transforms.resize(cable_count);
  update_transform();
}

bool PowerLineCable::operator==(const PowerLineCable& other) const noexcept {
  return pole_a == other.pole_a && pole_b == other.pole_b;
}

void PowerLineCable::update_transform() noexcept {
  std::vector<glm::vec3> a_points = pole_a.get_cable_snapping_points();
  std::vector<glm::vec3> b_points = pole_b.get_cable_snapping_points();

  glm::vec2 a1(a_points[0].x, a_points[0].z);
  glm::vec2 a4(a_points[3].x, a_points[3].z);
  glm::vec2 b1(b_points[0].x, b_points[0].z);
  glm::vec2 b4(b_points[3].x, b_points[3].z);

  if (geometry::line_segments_intersect(a1, b1, a4, b4)) {
    std::swap(b_points[0], b_points[3]);
    std::swap(b_points[1], b_points[2]);
  }

  for (size_t i = 0; i < cable_count; i++) {
    const auto& a = a_points[i];
    const auto& b = b_points[i];

    const float distance = glm::distance(a, b);
    const glm::vec3 cable_pos = (a + b) * 0.5f;
    const glm::vec3 delta = (a - b);
    const float angle = atan2f(delta.x, delta.z);

    glm::mat4 translation = glm::translate(glm::mat4(1), cable_pos);
    glm::mat4 rotation = glm::rotate(translation, angle, glm::vec3(0, 1, 0));
    glm::mat4 scale = glm::scale(rotation, glm::vec3(1, 1, distance));

    transforms[i] = scale;
  }
}

[[nodiscard]] const std::vector<glm::mat4>& PowerLineCable::get_transforms() const noexcept {
  return transforms;
}
}