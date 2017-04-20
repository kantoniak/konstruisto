#ifndef DATA_DIRECTION_HPP
#define DATA_DIRECTION_HPP

#include "glm/glm.hpp"

namespace data {
enum class Direction { N, W, S, E };

const glm::ivec2 toVector(const Direction dir);
}

#endif