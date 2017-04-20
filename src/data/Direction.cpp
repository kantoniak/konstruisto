#include "Direction.hpp"

namespace data {

const glm::ivec2 toVector(const Direction dir) {
  switch (dir) {
  case Direction::N:
    return glm::ivec2(1, 0);
  case Direction::S:
    return glm::ivec2(-1, 0);
  case Direction::W:
    return glm::ivec2(0, 1);
  case Direction::E:
    return glm::ivec2(0, -1);
  }
}
}