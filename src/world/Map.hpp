#ifndef WORLD_MAP_HPP
#define WORLD_MAP_HPP

#include <glm/glm.hpp>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../data/Chunk.hpp"
#include "../data/City.hpp"
#include "../data/Road.hpp"

namespace world {

class Map {
  typedef const std::vector<data::Chunk*> chunkList;
  typedef std::vector<data::Chunk*>::const_iterator chunkListIter;

public:
  Map();
  void cleanup();

  void createChunk(glm::ivec2 position);
  void loadChunk(data::Chunk & chunk);
  void setChunkNeighbors(data::Chunk& chunk);

  unsigned int getChunksCount();
  chunkList getChunks();
  const data::Chunk& getChunk(glm::ivec2 chunkPosition) const;
  bool chunkExists(glm::ivec2 chunkPosition);
  chunkListIter getChunkIterator();

  bool addLot(data::Lot lot);

  void addBuilding(data::buildings::Building building);
  unsigned int getBuildingCount();

  // TODO(kantoniak): Map::setCurrentCity() - change parameter to ObjId one day
  void setCurrentCity(data::City* city);
  data::City& getCurrentCity();

  void addRoads(const std::vector<data::Road> roads);

  void removeBuilding(data::buildings::Building building);

protected:
  std::vector<data::Chunk*> chunks;
  data::City* currentCity;

  // Cached
  unsigned int buildingCount;

  data::Chunk& getNonConstChunk(glm::ivec2 chunkPosition) const;
};
}

#endif
