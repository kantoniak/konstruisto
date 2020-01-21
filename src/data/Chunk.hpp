#ifndef DATA_CHUNK_HPP
#define DATA_CHUNK_HPP

#include <algorithm>
#include <glm/glm.hpp>
#include <vector>

#include "Building.hpp"
#include "Lot.hpp"
#include "RoadGraph.hpp"
#include "TreeGroup.hpp"

namespace data {

class Chunk {
  using lotList = const std::vector<data::Lot>;

public:
  constexpr unsigned static int SIDE_LENGTH = 64;

  Chunk();
  Chunk(const Chunk& chunk);

  void setObjectId(unsigned int objectId);

  void setPosition(glm::ivec2 position);
  [[nodiscard]] glm::ivec2 getPosition() const;

  void setNeighborN(Chunk* neigborN);
  void setNeighborS(Chunk* neigborS);
  void setNeighborW(Chunk* neigborW);
  void setNeighborE(Chunk* neigborE);

  void addRoad(data::Road road);
  void updateRoadGraph(const std::vector<data::Position<int32_t>>& tiles);
  [[nodiscard]] const RoadGraph<SIDE_LENGTH>& getRoadGraph() const;

  void addLot(data::Lot lot);
  [[nodiscard]] lotList getLots() const;

  // Buildings
  void add_building(data::Building::ptr building) noexcept;
  bool remove_building(const data::Building& building) noexcept;
  [[nodiscard]] const std::vector<data::Building::ptr>& get_buildings() const noexcept;

  // Trees
  void add_tree(data::Tree::ptr tree) noexcept;
  bool remove_tree(const data::Tree& tree) noexcept;
  void age_trees(float delta_in_turns) noexcept;
  [[nodiscard]] const TreeGroup& get_trees() const noexcept;

  template <class Archive>
  void save(Archive& archive) const {
    archive(position);
    archive(roadGraph);
    archive(residential);
  }

  template <class Archive>
  void load(Archive& archive) {
    archive(position);
    archive(roadGraph);
    archive(residential);
  }

private:
  unsigned int objectId;
  glm::ivec2 position;

  RoadGraph<SIDE_LENGTH> roadGraph;

  std::vector<data::Building::ptr> residential;
  unsigned int residentialSize;

  std::vector<data::Lot> lots;
  TreeGroup trees;
};
}

#endif