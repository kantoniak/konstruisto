#ifndef DATA_POSITION_HPP
#define DATA_POSITION_HPP

#include <vector>

#include <glm/glm.hpp>

namespace data {

template <typename T>
class Position;

template <typename T>
bool operator==(const data::Position<T>& a, const data::Position<T>& b);

template <typename T>
class Position {
  using vec2 = glm::vec<2, T, glm::qualifier::defaultp>;

private:
  constexpr unsigned static int SIDE_LENGTH = 64; // FIXME(kantoniak): Use constant from data::Chunk
  vec2 global;

public:
  Position();
  Position(vec2 global);

  void setGlobal(vec2 global);
  void setLocal(vec2 local, vec2 chunk);
  [[nodiscard]] vec2 getLocal() const;
  [[nodiscard]] vec2 getLocal(const vec2 chunk) const;
  [[nodiscard]] unsigned int getLocalIndex() const;
  [[nodiscard]] vec2 getGlobal() const;
  [[nodiscard]] glm::ivec2 getChunk() const;
  [[nodiscard]] std::vector<Position> getNeighbors() const;

  friend bool operator==(const Position& a, const Position& b) {
    return a.getGlobal() == b.getGlobal();
  }
};
}

namespace std {

template <typename T>
struct hash<data::Position<T>> {
  std::size_t operator()(const data::Position<T>& p) const {
    typename data::Position<T>::vec2 globalPos = p.getGlobal();
    return std::hash<int>()(globalPos.x) ^ std::hash<int>()(globalPos.y);
  }
};
}

#endif