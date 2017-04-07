#ifndef DATA_POSITION_HPP
#define DATA_POSITION_HPP

#include "glm/glm.hpp"

namespace data {

struct Position {
private:
  glm::ivec2 global;

public:
  void setGlobal(glm::ivec2 global);
  void setLocal(glm::ivec2 local, glm::ivec2 chunk);
  glm::ivec2 getLocal() const;
  glm::ivec2 getGlobal() const;
  glm::ivec2 getChunk() const;
};
}

#endif