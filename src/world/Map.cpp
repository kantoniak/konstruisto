#include "Map.hpp"

namespace world {
Map::Map() {
  buildingCount = 0;
}

void Map::cleanup() {
  for (auto it = chunks.begin(); chunks.end() != it; it++) {
    delete *it;
  }
  chunks.clear();
  buildingCount = 0;
}

void Map::createChunk(glm::ivec2 position) {
  // TODO(kantoniak): Check if chunk exists already
  auto* chunk = new data::Chunk();
  chunk->setPosition(position);
  chunks.push_back(chunk);
  this->setChunkNeighbors(*chunk);
}

void Map::loadChunk(data::Chunk& chunk) {
  auto* ptr = new data::Chunk(chunk);
  chunks.push_back(ptr);
  this->setChunkNeighbors(*ptr);
  buildingCount += ptr->getResidentialSize();
}

void Map::setChunkNeighbors(data::Chunk& chunk) {
  glm::ivec2 position = chunk.getPosition();

  glm::ivec2 neighborPos = position;
  neighborPos.x = position.x + 1;
  if (chunkExists(neighborPos)) {
    data::Chunk& neighbor = getNonConstChunk(neighborPos);
    chunk.setNeighborE(&neighbor);
    neighbor.setNeighborW(&chunk);
  }

  neighborPos.x = position.x - 1;
  if (chunkExists(neighborPos)) {
    data::Chunk& neighbor = getNonConstChunk(neighborPos);
    chunk.setNeighborW(&neighbor);
    neighbor.setNeighborE(&chunk);
  }

  neighborPos.x = position.x;
  neighborPos.y = position.y + 1;
  if (chunkExists(neighborPos)) {
    data::Chunk& neighbor = getNonConstChunk(neighborPos);
    chunk.setNeighborS(&neighbor);
    neighbor.setNeighborN(&chunk);
  }

  neighborPos.y = position.y - 1;
  if (chunkExists(neighborPos)) {
    data::Chunk& neighbor = getNonConstChunk(neighborPos);
    chunk.setNeighborN(&neighbor);
    neighbor.setNeighborS(&chunk);
  }
}

unsigned int Map::getChunksCount() {
  return chunks.size();
}

Map::chunkList Map::getChunks() {
  return chunks;
}

const data::Chunk& Map::getChunk(glm::ivec2 chunkPosition) const {
  for (auto chunk : chunks) {
    if (glm::all(glm::equal(chunk->getPosition(), chunkPosition))) {
      return *chunk;
    }
  }
  throw std::invalid_argument("Chunk does not exist");
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

bool Map::addLot(data::Lot lot) {
  glm::ivec2 chunkPos = lot.position.getChunk();
  if (!chunkExists(chunkPos)) {
    return false;
  }
  getNonConstChunk(chunkPos).addLot(lot);
  return true;
}

void Map::addBuilding(data::buildings::Building building) {
  glm::ivec2 chunk = glm::ivec2(building.x, building.y) / (int)data::Chunk::SIDE_LENGTH;
  if (chunkExists(chunk)) {
    getNonConstChunk(chunk).addBuilding(building);
    buildingCount++;
  }
}

unsigned int Map::getBuildingCount() {
  return buildingCount;
}

void Map::setCurrentCity(data::City* city) {
  currentCity = city;
}

data::City& Map::getCurrentCity() {
  assert(currentCity != nullptr);
  return *currentCity;
}

void Map::addRoads(const std::vector<data::Road>& roads) {
  struct KeyFuncs {
    size_t operator()(const glm::ivec2& k) const {
      return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
    }

    bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
      return a.x == b.x && a.y == b.y;
    }
  };

  std::vector<data::Position<int32_t>> toUpdate;
  std::unordered_set<glm::ivec2, KeyFuncs, KeyFuncs> chunksToUpdate;

  for (auto road : roads) {
    const glm::ivec2 chunkPos = road.getTiles()[0].getChunk();
    if (!chunkExists(chunkPos)) {
      return;
    }
    getNonConstChunk(chunkPos).addRoad(road);

    for (auto& tile : road.getTiles()) {
      toUpdate.push_back(tile);
      chunksToUpdate.insert(tile.getChunk());
      for (auto& neighbor : tile.getNeighbors()) {
        toUpdate.push_back(neighbor);
        chunksToUpdate.insert(neighbor.getChunk());
      }
    }
  }

  for (auto& chunk : chunksToUpdate) {
    if (!chunkExists(chunk)) {
      continue;
    }

    std::vector<data::Position<int32_t>> fromCurrentChunk;

    for (auto& tile : toUpdate) {
      if (tile.getChunk() == chunk) {
        fromCurrentChunk.push_back(tile);
      }
    }

    getNonConstChunk(chunk).updateRoadGraph(fromCurrentChunk);
  }
}

void Map::removeBuilding(data::buildings::Building building) {
  for (data::Chunk* chunk : chunks) {
    if (chunk->removeBuilding(building)) {
      buildingCount--;
    }
  }
}

data::Chunk& Map::getNonConstChunk(glm::ivec2 chunkPosition) const {
  for (auto chunk : chunks) {
    if (glm::all(glm::equal(chunk->getPosition(), chunkPosition))) {
      return *chunk;
    }
  }
  throw std::invalid_argument("Chunk does not exist");
}
}