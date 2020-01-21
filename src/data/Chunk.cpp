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

void Chunk::add_building(data::Building::ptr building) noexcept {
  residential.push_back(building);
}

bool Chunk::remove_building(const data::Building& building) noexcept {
  auto to_remove = residential.end();
  for (auto it = residential.begin(); it != residential.end(); it++) {
    if ((*it)->x == building.x && (*it)->y == building.y) {
      to_remove = it;
    }
  }
  if (to_remove == residential.end()) {
    return false;
  }
  residential.erase(to_remove);
  return true;
}

const std::vector<data::Building::ptr>& Chunk::get_buildings() const noexcept {
  return residential;
}

void Chunk::add_tree(data::Tree::ptr tree) noexcept {
  trees.add_tree(tree);
}

bool Chunk::remove_tree(const Tree& tree) noexcept {
  return trees.remove_tree(tree);
}

void Chunk::age_trees(float delta_in_turns) noexcept {
  trees.update(delta_in_turns);
}

const TreeGroup& Chunk::get_trees() const noexcept {
  return trees;
}
}