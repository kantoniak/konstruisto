#include "Chunk.hpp"

namespace data {

Chunk::Chunk() {
  objectId = 0;
  position = glm::ivec2();
  residentialSize = 0;
}

Chunk::Chunk(const Chunk& chunk) : roadGraph(chunk.getRoadGraph()), residential(chunk.residential) {
  objectId = 0;
  position = chunk.getPosition();
  residentialSize = chunk.residential.size();
}

void Chunk::setObjectId(unsigned int objectId) {
  this->objectId = objectId;
}

void Chunk::setPosition(glm::ivec2 position) {
  this->position = position;
}

glm::ivec2 Chunk::getPosition() const {
  return position;
}

Chunk::residentialList Chunk::getResidentials() const {
  return residential;
}

Chunk::residentialListIter Chunk::getResidentialIterator() const {
  return residential.begin();
}

unsigned int Chunk::getResidentialSize() const {
  return residentialSize;
}

void Chunk::setNeighborN(Chunk* neigborN) {
  this->roadGraph.setNeighborN(&(neigborN->roadGraph));
}

void Chunk::setNeighborS(Chunk* neigborS) {
  this->roadGraph.setNeighborS(&(neigborS->roadGraph));
}

void Chunk::setNeighborW(Chunk* neigborW) {
  this->roadGraph.setNeighborW(&(neigborW->roadGraph));
}

void Chunk::setNeighborE(Chunk* neigborE) {
  this->roadGraph.setNeighborE(&(neigborE->roadGraph));
}

void Chunk::addRoad(data::Road road) {
  this->roadGraph.addRoad(road);
}

void Chunk::updateRoadGraph(const std::vector<data::Position<int32_t>>& tiles) {
  this->roadGraph.update(tiles);
}

const RoadGraph<Chunk::SIDE_LENGTH>& Chunk::getRoadGraph() const {
  return this->roadGraph;
}

void Chunk::addLot(data::Lot lot) {
  lots.push_back(lot);
}

Chunk::lotList Chunk::getLots() const {
  return lots;
}

void Chunk::addBuilding(data::buildings::Building building) {
  residential.push_back(building);
}

bool Chunk::removeBuilding(data::buildings::Building building) {
  auto toRemove = residential.end();
  for (auto it = residential.begin(); it != residential.end(); it++) {
    if ((*it).x == building.x && (*it).y == building.y) {
      toRemove = it;
    }
  }
  if (toRemove == residential.end()) {
    return false;
  }
  residential.erase(toRemove);
  return true;
}

void Chunk::add_tree(data::Tree tree) noexcept {
  trees.push_back(tree);
}

bool Chunk::remove_tree(const data::Tree& tree) noexcept {
  const auto& to_remove = std::find(trees.begin(), trees.end(), tree);

  if (to_remove == trees.end()) {
    return false;
  }

  trees.erase(to_remove);
  return false;
}

void Chunk::age_trees(float delta) noexcept {
  for (auto& tree : trees) {
    tree.add_age(delta);
  }
}

const std::vector<data::Tree>& Chunk::get_trees() const noexcept {
  return trees;
}
}