#ifndef DATA_POSITION_HPP
#define DATA_POSITION_HPP

#include <glm/glm.hpp>
#include <vector>

namespace data {

struct Position {
private:
  glm::ivec2 global;

public:
  Position();
  Position(glm::ivec2 global);
  void setGlobal(glm::ivec2 global);
  void setLocal(glm::ivec2 local, glm::ivec2 chunk);
  glm::ivec2 getLocal() const;
  glm::ivec2 getLocal(const glm::ivec2 chunk) const;
  unsigned int getLocalIndex() const;
  glm::ivec2 getGlobal() const;
  glm::ivec2 getChunk() const;
  std::vector<Position> getNeighbors() const;

  friend bool operator==(const Position& a, const Position& b);
};
}

namespace std {

template <> struct hash<data::Position> {
  std::size_t operator()(const data::Position& p) const {
    glm::ivec2 globalPos = p.getGlobal();
    return std::hash<int>()(globalPos.x) ^ std::hash<int>()(globalPos.y);
  }
};
}

#endif