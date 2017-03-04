#ifndef WORLD_MAP_HPP
#define WORLD_MAP_HPP

#include <glm/glm.hpp>
#include <stdexcept>
#include <vector>

#include "../data/Chunk.hpp"

namespace world {

class Map {
  typedef const std::vector<data::Chunk*> chunkList;
  typedef std::vector<data::Chunk*>::const_iterator chunkListIter;

public:
  Map();
  void cleanup();

  void createChunk(glm::ivec2 position);
  void randomizeChunk(glm::ivec2 position);

  unsigned int getChunksCount();
  chunkList getChunks();
  bool chunkExists(glm::ivec2 chunkPosition);
  chunkListIter getChunkIterator();

  void addBuilding(data::buildings::Building building);
  unsigned int getBuildingCount();

protected:
  std::vector<data::Chunk*> chunks;

  // Cached
  unsigned int buildingCount;

  data::Chunk& getChunk(glm::ivec2 chunkPosition);
};
}

#endif
