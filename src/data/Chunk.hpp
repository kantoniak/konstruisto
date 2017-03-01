#ifndef DATA_CHUNK_HPP
#define DATA_CHUNK_HPP

#include <glm/glm.hpp>
#include <vector>

#include "buildings.hpp"

namespace data {

class Chunk {
  typedef const std::vector<data::buildings::Building> residentialList;
  typedef std::vector<data::buildings::Building>::const_iterator residentialListIter;

public:
  unsigned const static int SIDE_LENGTH = 16;

  Chunk();
  void setObjectId(unsigned int objectId);

  void setPosition(glm::ivec2 position);
  glm::ivec2 getPosition();

  void randomizeBuildings();
  residentialList getResidentials();
  residentialListIter getResidentialIterator();
  unsigned int getResidentialSize();

private:
  unsigned int objectId;
  glm::ivec2 position;

  std::vector<data::buildings::Building> residential;
  unsigned int residentialSize;
};
}

#endif