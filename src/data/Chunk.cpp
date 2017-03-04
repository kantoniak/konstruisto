#include "Chunk.hpp"

namespace data {

Chunk::Chunk() {
  objectId = 0;
  position = glm::ivec2();
  residentialSize = 0;
}

void Chunk::setObjectId(unsigned int objectId) {
  this->objectId = objectId;
}

void Chunk::setPosition(glm::ivec2 position) {
  this->position = position;
}

glm::ivec2 Chunk::getPosition() {
  return position;
}

void Chunk::randomizeBuildings() {
  constexpr unsigned int buildingCount = (SIDE_LENGTH * SIDE_LENGTH / 4) * 0.1f;
  residential.reserve(buildingCount);
  residentialSize = buildingCount;

  constexpr unsigned int positionMax = SIDE_LENGTH / 2;
  for (unsigned int i = 0; i < buildingCount; i++) {
    residential.push_back(data::buildings::Building());
    residential[i].x = (rand() % positionMax) * 2 + position.x * SIDE_LENGTH;
    residential[i].y = (rand() % positionMax) * 2 + position.y * SIDE_LENGTH;
    residential[i].width = (rand() % 3 + 1) * 2;
    residential[i].length = (rand() % 3 + 1) * 2;
    residential[i].level = rand() % 6 + 1;
  }
}

Chunk::residentialList Chunk::getResidentials() {
  return residential;
}

Chunk::residentialListIter Chunk::getResidentialIterator() {
  return residential.begin();
}

unsigned int Chunk::getResidentialSize() {
  return residentialSize;
}

void Chunk::addBuilding(data::buildings::Building building) {
  residential.push_back(building);
}
}