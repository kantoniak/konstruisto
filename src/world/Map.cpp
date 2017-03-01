#include "Map.hpp"

namespace world {
Map::Map() {
  size = glm::ivec2(0, 0);
}

void Map::cleanup() {
  for (auto it = chunks.begin(); chunks.end() != it; it++) {
    delete *it;
  }
  chunks.clear();
}

void Map::createRandom(glm::ivec2 sizeInChunks) {
  this->size = sizeInChunks;
  const unsigned int chunksCount = sizeInChunks.x * sizeInChunks.y;
  chunks.reserve(chunksCount);

  buildingCount = 0;

  for (unsigned int it = 0; it < chunksCount; it++) {
    data::Chunk* chunk = new data::Chunk;
    chunk->setPosition(glm::ivec2(it % size.x, it / size.x));
    chunk->randomizeBuildings();
    chunks.push_back(chunk);

    buildingCount += chunk->getResidentialSize();
  }
}

glm::ivec2 Map::getSize() {
  return size * (int)data::Chunk::SIDE_LENGTH;
}

glm::ivec2 Map::getSizeInChunks() {
  return size;
}

unsigned int Map::getChunksCount() {
  return size.x * size.y;
}

Map::chunkList Map::getChunks() {
  return chunks;
}

bool Map::chunkExists(glm::ivec2 chunkPosition) {
  for (data::Chunk* chunk : getChunks()) {
    if (glm::all(glm::equal(chunk->getPosition(), chunkPosition))) {
      return true;
    }
  }
  return false;
}

Map::chunkListIter Map::getChunkIterator() {
  return chunks.begin();
}

unsigned int Map::getBuildingCount() {
  return buildingCount;
}
}