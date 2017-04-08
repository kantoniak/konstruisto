#ifndef DATA_CHUNK_HPP
#define DATA_CHUNK_HPP

#include <algorithm>
#include <glm/glm.hpp>
#include <vector>

#include "Lot.hpp"
#include "Road.hpp"
#include "RoadGraph.hpp"
#include "buildings.hpp"

namespace data {

class Chunk {
  typedef const std::vector<data::Lot> lotList;
  typedef const std::vector<data::buildings::Building> residentialList;
  typedef std::vector<data::buildings::Building>::const_iterator residentialListIter;

public:
  constexpr unsigned static int SIDE_LENGTH = 64;

  Chunk();
  void setObjectId(unsigned int objectId);

  void setPosition(glm::ivec2 position);
  glm::ivec2 getPosition();

  residentialList getResidentials();
  residentialListIter getResidentialIterator();
  unsigned int getResidentialSize();

  void addLot(data::Lot lot);
  lotList getLots();

  void addBuilding(data::buildings::Building building);
  bool removeBuilding(data::buildings::Building building);

  void addRoad(Road road);
  const std::vector<Road>& getRoads();

private:
  unsigned int objectId;
  glm::ivec2 position;

  std::vector<data::buildings::Building> residential;
  unsigned int residentialSize;

  std::vector<data::Lot> lots;

  RoadGraph roadGraph;
};
}

#endif