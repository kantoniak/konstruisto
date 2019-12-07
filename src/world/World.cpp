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

  float age_delta = timer.delta_to_turns(delta);
  for (auto chunk_ptr : getMap().getChunks()) {
    data::Chunk& chunk = *chunk_ptr;
    chunk.age_trees(age_delta);
  }
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