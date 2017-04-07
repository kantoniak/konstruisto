#ifndef DATA_ROADS_HPP
#define DATA_ROADS_HPP

#include "glm/glm.hpp"
#include "glm/gtc/integer.hpp"

#include "Direction.hpp"
#include "Position.hpp"

namespace data {
namespace roads {

struct RoadDefinition {
  int typeId = -1;
  int width;
};

const struct RoadDefinition Standard = {1, 2};

struct Road {
  int type;
  Position position;
  Direction direction;
  unsigned short length;
};

const RoadDefinition getDefinition(Road road);
}
}

#endif