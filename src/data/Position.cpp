#include "Position.hpp"

namespace data {
Position::Position() = default;

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

unsigned int Position::getLocalIndex() const {
  const glm::ivec2 local = getLocal();
  return local.x + local.y * 64;
}

glm::ivec2 Position::getChunk() const {
  glm::ivec2 result = global / 64;
  if (global.x < 0) {
    result.x--;
  }
  if (global.y < 0) {
    result.y--;
  }
  return result;
}

std::vector<Position> Position::getNeighbors() const {
  std::vector<Position> result;
  result.emplace_back(global + glm::ivec2(1, -1));
  result.emplace_back(global + glm::ivec2(1, 0));
  result.emplace_back(global + glm::ivec2(1, 1));
  result.emplace_back(global + glm::ivec2(0, -1));
  result.emplace_back(global + glm::ivec2(0, +1));
  result.emplace_back(global + glm::ivec2(-1, -1));
  result.emplace_back(global + glm::ivec2(-1, 0));
  result.emplace_back(global + glm::ivec2(-1, +1));
  return result;
}
}

bool operator==(const data::Position& a, const data::Position& b) {
  return a.getGlobal() == b.getGlobal();
}