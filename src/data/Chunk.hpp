#ifndef DATA_CHUNK_HPP
#define DATA_CHUNK_HPP

#include <glm/glm.hpp>
#include <vector>

#include "buildings.hpp"
#include "roads.hpp"

namespace data {

class Chunk {
  typedef const std::vector<data::buildings::Building> residentialList;
  typedef std::vector<data::buildings::Building>::const_iterator residentialListIter;
  typedef const std::vector<data::roads::Road> roadList;

public:
  constexpr unsigned static int SIDE_LENGTH = 64;

  Chunk();
  void setObjectId(unsigned int objectId);

  void setPosition(glm::ivec2 position);
  glm::ivec2 getPosition();

  void randomizeBuildings();
  residentialList getResidentials();
  residentialListIter getResidentialIterator();
  unsigned int getResidentialSize();

  void addBuilding(data::buildings::Building building);

  void addRoad(data::roads::Road road);
  roadList getRoads();

private:
  unsigned int objectId;
  glm::ivec2 position;

  std::vector<data::buildings::Building> residential;
  unsigned int residentialSize;

  std::vector<data::roads::Road> roads;
};
}

#endif