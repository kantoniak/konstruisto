#ifndef WORLD_MAP_HPP
#define WORLD_MAP_HPP

#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <glm/glm.hpp>

#include "../data/Chunk.hpp"
#include "../data/City.hpp"
#include "../data/ElectricityGrid.hpp"
#include "../data/Road.hpp"

namespace world {

class Map {
  using chunkList = const std::vector<data::Chunk*>;
  using chunkListIter = std::vector<data::Chunk*>::const_iterator;

public:
  Map();
  void cleanup();

  void createChunk(glm::ivec2 position);
  void loadChunk(data::Chunk& chunk);
  void setChunkNeighbors(data::Chunk& chunk);

  unsigned int getChunksCount();
  chunkList getChunks();
  [[nodiscard]] const data::Chunk& getChunk(glm::ivec2 chunkPosition) const;
  bool chunkExists(glm::ivec2 chunkPosition);
  chunkListIter getChunkIterator();

  bool addLot(data::Lot lot);

  // TODO(kantoniak): Map::setCurrentCity() - change parameter to ObjId one day
  void setCurrentCity(data::City* city);
  data::City& getCurrentCity();

  void addRoads(const std::vector<data::Road>& roads);

  // Buildings
  void add_building(data::Building::ptr building) noexcept;
  void remove_building(const data::Building& building) noexcept;
  [[nodiscard]] size_t get_building_count() const noexcept;

  // Trees
  void add_tree(data::Tree::ptr tree_ptr) noexcept;
  bool remove_tree(const data::Tree& tree) noexcept;

  // Electricity
  [[nodiscard]] const data::ElectricityGrid& get_electricity_grid() const noexcept;
  void add_power_pole(data::PowerLinePole::ptr pole_ptr) noexcept;
  bool remove_power_pole(const data::PowerLinePole& pole) noexcept;

  void add_power_cable(data::PowerLineCable::ptr cable_ptr) noexcept;
  bool remove_power_cable(const data::PowerLineCable& cable) noexcept;

protected:
  std::vector<data::Chunk*> chunks;
  data::City* currentCity;
  data::ElectricityGrid electricity_grid;

  // Cached
  size_t building_count;

  [[nodiscard]] data::Chunk& getNonConstChunk(glm::ivec2 chunkPosition) const;
};
}

#endif
