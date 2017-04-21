#include "Road.hpp"

namespace data {

void Road::setType(const RoadType& type) {
  this->type = type.id;
}

const RoadType Road::getType() const {
  switch (type) {
  case RoadTypes.Standard.id:
    return RoadTypes.Standard;
    break;
  default:
    return RoadType{};
  }
}

const glm::ivec2 Road::getEnd() const {
  return position.getGlobal() + glm::ivec2((direction == data::Direction::W ? length : getType().width) - 1,
                                           (direction == data::Direction::N ? length : getType().width) - 1);
}
}
