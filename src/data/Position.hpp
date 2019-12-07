#ifndef DATA_POSITION_HPP
#define DATA_POSITION_HPP

#include <glm/glm.hpp>
#include <vector>

namespace data {

template <typename T>
struct Position {
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
  [[nodiscard]] vec2 getChunk() const;
  [[nodiscard]] std::vector<Position> getNeighbors() const;

  template <typename T>
  friend bool operator==(const Position<T>& a, const Position<T>& b);
};
}

namespace std {

template <typename T>
struct hash<data::Position<T>> {
  std::size_t operator()(const data::Position<T>& p) const {
    vec2 globalPos = p.getGlobal();
    return std::hash<int>()(globalPos.x) ^ std::hash<int>()(globalPos.y);
  }
};
}

#endif