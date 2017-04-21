#ifndef DATA_ROAD_HPP
#define DATA_ROAD_HPP

#include "glm/glm.hpp"
#include "glm/gtc/integer.hpp"

#include "Direction.hpp"
#include "Position.hpp"

namespace data {

struct RoadType {
  unsigned int id;
  int width;
};

constexpr struct {
  const struct RoadType Standard = {1, 2};
} RoadTypes;

struct Road {
private:
  unsigned int type;

public:
  Position position;
  Direction direction;
  unsigned short length;

  void setType(const RoadType& type);
  const RoadType getType() const;
  const glm::ivec2 getEnd() const;
};
}

#endif