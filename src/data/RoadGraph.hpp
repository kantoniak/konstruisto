#ifndef DATA_ROADGRAPH_HPP
#define DATA_ROADGRAPH_HPP

#include <vector>

#include <cereal/types/vector.hpp>

#include "Layer.hpp"
#include "Position.hpp"
#include "Road.hpp"

namespace data {

template <unsigned int L>
class RoadGraph : public Layer<char, L> {

public:
  constexpr static char NO_ROAD = 0;
  constexpr static char ROAD_NONE = 1 | 2 | 4 | 8;
  constexpr static char ROAD_N = 2 | 4 | 8;
  constexpr static char ROAD_S = 1 | 4 | 8;
  constexpr static char ROAD_W = 1 | 2 | 8;
  constexpr static char ROAD_E = 1 | 2 | 4;
  constexpr static char ROAD_WE = 1 | 2;
  constexpr static char ROAD_SE = 1 | 4;
  constexpr static char ROAD_SW = 1 | 8;
  constexpr static char ROAD_NE = 2 | 4;
  constexpr static char ROAD_NW = 2 | 8;
  constexpr static char ROAD_NS = 4 | 8;
  constexpr static char ROAD_NSW = 8;
  constexpr static char ROAD_NSE = 4;
  constexpr static char ROAD_NWE = 2;
  constexpr static char ROAD_SWE = 1;
  constexpr static char ROAD_NSWE = 16;

  RoadGraph();
  RoadGraph(const RoadGraph& roadGraph);

  void setNeighborN(RoadGraph* neigborN);
  void setNeighborS(RoadGraph* neigborS);
  void setNeighborW(RoadGraph* neigborW);
  void setNeighborE(RoadGraph* neigborE);

  void addRoad(const data::Road& road);
  void update(const std::vector<data::Position<int32_t>>& tiles);

  template <class Archive>
  void save(Archive& archive) const {
    archive(Layer<char, L>::layerData);
  }

  template <class Archive>
  void load(Archive& archive) {
    archive(Layer<char, L>::layerData);
  }

private:
  RoadGraph* neighborN;
  RoadGraph* neighborS;
  RoadGraph* neighborW;
  RoadGraph* neighborE;

  void update();
  void updateIndex(unsigned int i);

  [[nodiscard]] bool noRoadToN(unsigned int i) const;
  [[nodiscard]] bool noRoadToS(unsigned int i) const;
  [[nodiscard]] bool noRoadToW(unsigned int i) const;
  [[nodiscard]] bool noRoadToE(unsigned int i) const;
};
}

#endif