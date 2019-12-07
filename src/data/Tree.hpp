#ifndef DATA_TREE_HPP
#define DATA_TREE_HPP

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "Position.hpp"

namespace data {

class Tree {
public:
  Tree(Position<float> position, float rotation, float age) noexcept;

  bool operator==(const Tree& other) const noexcept;

  void add_age(float age_delta) noexcept;

  [[nodiscard]] Position<float> get_position() const noexcept;
  [[nodiscard]] glm::mat4 get_transform() const noexcept;

private:
  Position<float> position;
  float rotation_angle;
  float age;
  glm::mat4 translation;
  glm::mat4 rotation;
  glm::mat4 transform;

  void init_matrices() noexcept;
  void update_transform() noexcept;
};
}

#endif