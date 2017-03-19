#ifndef DATA_ROADS_HPP
#define DATA_ROADS_HPP

namespace data {
namespace roads {

struct RoadDefinition {
  int typeId = -1;
  int width;
};

const struct RoadDefinition Standard = {1, 2};

enum class Direction {N, W}; 

struct Road {
  int type;
  long x;
  long y;
  Direction direction;
  unsigned short length;
};

const RoadDefinition getDefinition(Road road);
}
}

#endif