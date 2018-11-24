#ifndef DATA_CHUNK_HPP
#define DATA_CHUNK_HPP

#include <algorithm>
#include <glm/glm.hpp>
#include <vector>

#include "Lot.hpp"
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
  Chunk(const Chunk& chunk);

  void setObjectId(unsigned int objectId);

  void setPosition(glm::ivec2 position);
  glm::ivec2 getPosition() const;

  residentialList getResidentials() const;
  residentialListIter getResidentialIterator() const;
  unsigned int getResidentialSize() const;

  void setNeighborN(Chunk* neigborN);
  void setNeighborS(Chunk* neigborS);
  void setNeighborW(Chunk* neigborW);
  void setNeighborE(Chunk* neigborE);

  void addRoad(data::Road road);
  void updateRoadGraph(const std::vector<data::Position>& tiles);
  const RoadGraph<SIDE_LENGTH>& getRoadGraph() const;

  void addLot(data::Lot lot);
  lotList getLots() const;

  void addBuilding(data::buildings::Building building);
  bool removeBuilding(data::buildings::Building building);

  template <class Archive> void save(Archive& archive) const {
    archive(position);
    archive(roadGraph);
    archive(residential);
  }

  template <class Archive> void load(Archive& archive) {
    archive(position);
    archive(roadGraph);
    archive(residential);
  }

private:
  unsigned int objectId;
  glm::ivec2 position;

  RoadGraph<SIDE_LENGTH> roadGraph;

  std::vector<data::buildings::Building> residential;
  unsigned int residentialSize;

  std::vector<data::Lot> lots;
};
}

#endif