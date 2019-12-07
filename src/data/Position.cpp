#include "Position.hpp"

namespace data {
template <typename T>
Position<T>::Position() = default;

template <typename T>
Position<T>::Position(vec2 global) : global(global) {
}

template <typename T>
void Position<T>::setGlobal(vec2 global) {
  this->global = global;
}

template <typename T>
void Position<T>::setLocal(vec2 local, vec2 chunk) {
  this->global = chunk * static_cast<T>(SIDE_LENGTH) + local;
}

template <typename T>
typename Position<T>::vec2 Position<T>::getLocal() const {
  return glm::mod(glm::vec2(global), glm::vec2(1, 1) * (float)static_cast<int>(SIDE_LENGTH));
}

template <typename T>
typename Position<T>::vec2 Position<T>::getLocal(const vec2 chunk) const {
  return global - vec2(chunk.x * static_cast<int>(SIDE_LENGTH), chunk.y * static_cast<int>(SIDE_LENGTH));
}

template <typename T>
typename Position<T>::vec2 Position<T>::getGlobal() const {
  return global;
}

template <typename T>
unsigned int Position<T>::getLocalIndex() const {
  const vec2 local = getLocal();
  return local.x + local.y * static_cast<int>(SIDE_LENGTH);
}

template <typename T>
typename glm::ivec2 Position<T>::getChunk() const {
  vec2 result = glm::ivec2(static_cast<int>(global.x / SIDE_LENGTH), static_cast<int>(global.y / SIDE_LENGTH));
  if (global.x < 0) {
    result.x--;
  }
  if (global.y < 0) {
    result.y--;
  }
  return result;
}

template <typename T>
std::vector<Position<T>> Position<T>::getNeighbors() const {
  std::vector<Position> result;
  result.emplace_back(global + vec2(1, -1));
  result.emplace_back(global + vec2(1, 0));
  result.emplace_back(global + vec2(1, 1));
  result.emplace_back(global + vec2(0, -1));
  result.emplace_back(global + vec2(0, +1));
  result.emplace_back(global + vec2(-1, -1));
  result.emplace_back(global + vec2(-1, 0));
  result.emplace_back(global + vec2(-1, +1));
  return result;
}

template class Position<int32_t>;
template class Position<float>;
}