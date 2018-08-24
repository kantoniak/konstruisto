#include "Position.hpp"

namespace data {
Position::Position() {
}

Position::Position(glm::ivec2 global) : global(global) {
}

void Position::setGlobal(glm::ivec2 global) {
  this->global = global;
}

void Position::setLocal(glm::ivec2 local, glm::ivec2 chunk) {
  this->global = chunk * 64 + local;
}

glm::ivec2 Position::getLocal() const {
  return glm::mod(glm::vec2(global), glm::vec2(1, 1) * (float)64);
}

glm::ivec2 Position::getLocal(const glm::ivec2 chunk) const {
  return global - glm::ivec2(chunk.x * 64, chunk.y * 64);
}

glm::ivec2 Position::getGlobal() const {
  return global;
}

glm::ivec2 Position::getChunk() const {
  return global / 64;
}
}