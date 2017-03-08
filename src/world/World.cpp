#include "World.hpp"

namespace world {
void World::init() {
}

void World::cleanup() {
  map.cleanup();
}

Camera& World::getCamera() {
  return camera;
}

Map& World::getMap() {
  return map;
}

Timer& World::getTimer() {
  return timer;
}
}