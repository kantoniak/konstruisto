#ifndef DATA_ROADGRAPH_HPP
#define DATA_ROADGRAPH_HPP

#include "Layer.hpp"
#include "Road.hpp"

namespace data {

class RoadGraph : public Layer<char> {

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

  RoadGraph(unsigned int sideLength);

  void setNeighborN(RoadGraph* neigborN);
  void setNeighborS(RoadGraph* neigborS);
  void setNeighborW(RoadGraph* neigborW);
  void setNeighborE(RoadGraph* neigborE);

  void addRoad(const data::Road road);

private:
  RoadGraph* neighborN;
  RoadGraph* neighborS;
  RoadGraph* neighborW;
  RoadGraph* neighborE;

  void update();
  void updateIndex(unsigned int i);

  bool noRoadToN(unsigned int i) const;
  bool noRoadToS(unsigned int i) const;
  bool noRoadToW(unsigned int i) const;
  bool noRoadToE(unsigned int i) const;
};
}

#endif