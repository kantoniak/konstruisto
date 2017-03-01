#ifndef WORLD_MAP_HPP
#define WORLD_MAP_HPP

#include <glm/glm.hpp>
#include <vector>

#include "../data/Chunk.hpp"

namespace world {

class Map {
  typedef const std::vector<data::Chunk*> chunkList;
  typedef std::vector<data::Chunk*>::const_iterator chunkListIter;

public:
  Map();
  void cleanup();

  void createRandom(glm::ivec2 sizeInChunks);

  glm::ivec2 getSize();
  glm::ivec2 getSizeInChunks();

  unsigned int getChunksCount();
  chunkList getChunks();
  bool chunkExists(glm::ivec2 chunkPosition);
  chunkListIter getChunkIterator();

  unsigned int getBuildingCount();

protected:
  glm::ivec2 size;
  std::vector<data::Chunk*> chunks;

  // Cached
  unsigned int buildingCount;
};
}

#endif
