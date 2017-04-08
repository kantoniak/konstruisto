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
}
