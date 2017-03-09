#include "World.hpp"

namespace world {
void World::init() {
  getTimer().init();
}

void World::cleanup() {
  map.cleanup();
}

void World::update(std::chrono::milliseconds delta) {
  getTimer().update(delta);
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