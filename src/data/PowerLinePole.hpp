#ifndef DATA_POWERLINEPOLE_HPP
#define DATA_POWERLINEPOLE_HPP

#include <memory>
#include <vector>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "Position.hpp"

namespace data {

class PowerLinePole {
public:
  using ptr = std::shared_ptr<PowerLinePole>;

  PowerLinePole(Position<float> position) noexcept;
  PowerLinePole(const PowerLinePole& other) noexcept = default;
  bool operator==(const PowerLinePole& other) const noexcept;

  [[nodiscard]] Position<float> get_position() const noexcept;
  [[nodiscard]] glm::mat4 get_transform() const noexcept;

  void set_translation(Position<float> position) noexcept;
  void set_rotation(float rotation) noexcept;

  // TODO(kantoniak): Docs
  [[nodiscard]] size_t get_cable_snapping_points_count() const noexcept;
  [[nodiscard]] const std::vector<glm::vec3>& get_cable_snapping_points() const noexcept;

private:
  Position<float> position;

  glm::mat4 translation;
  glm::mat4 rotation;
  glm::mat4 transform;

  const std::vector<glm::vec3> cable_snapping_points;
  const size_t snapping_points_count;
  std::vector<glm::vec3> transformed_cable_snapping_points;

  void init_matrices() noexcept;
  void update_transform() noexcept;
};
}

#endif