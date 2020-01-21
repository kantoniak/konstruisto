#ifndef DATA_TREE_HPP
#define DATA_TREE_HPP

#include <array>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "../geometry/Collidable.hpp"
#include "Position.hpp"

namespace data {

class Tree {
public:
  enum Type { GREEN, ORANGE, MODEL2 };
  static constexpr size_t TREE_TYPE_COUNT = 3;
  static constexpr std::array<Type, TREE_TYPE_COUNT> TYPES = {GREEN, ORANGE, MODEL2};

  Tree(Type type, Position<float> position, float rotation, float age, geometry::Collidable::ptr body) noexcept;

  bool operator==(const Tree& other) const noexcept;

  void add_age(float age_delta) noexcept;

  [[nodiscard]] Type get_type() const noexcept;
  [[nodiscard]] Position<float> get_position() const noexcept;
  [[nodiscard]] glm::mat4 get_transform() const noexcept;
  [[nodiscard]] geometry::Collidable::ptr get_body() const noexcept;

private:
  Type type;
  Position<float> position;
  float rotation_angle;
  float age;
  geometry::Collidable::ptr body;

  glm::mat4 translation;
  glm::mat4 rotation;
  glm::mat4 transform;

  void init_matrices() noexcept;
  void update_transform() noexcept;
};
}

#endif