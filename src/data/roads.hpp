#ifndef DATA_ROADS_HPP
#define DATA_ROADS_HPP

namespace data {
namespace roads {

struct RoadDefinition {
  int typeId = -1;
  int width;
};

const struct RoadDefinition Standard = {1, 2};

struct Road {
  int type;
  long x;
  long y;
  unsigned short width;
  unsigned short length;
};
}
}

#endif