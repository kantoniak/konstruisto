#ifndef DATA_ROADS_HPP
#define DATA_ROADS_HPP

#include "glm/glm.hpp"
#include "glm/gtc/integer.hpp"

namespace data {
namespace roads {

struct Position {
private:
  glm::ivec2 global;

public:
  void setGlobal(glm::ivec2 global) {
    this->global = global;
  }
  void setLocal(glm::ivec2 local, glm::ivec2 chunk) {
    this->global = chunk * 64 + local;
  }
  glm::ivec2 getLocal() const {
    return glm::mod(glm::vec2(global), glm::vec2(1, 1) * (float)64);
  }
  glm::ivec2 getGlobal() const {
    return global;
  }
};

struct RoadDefinition {
  int typeId = -1;
  int width;
};

const struct RoadDefinition Standard = {1, 2};

enum class Direction { N, W };

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