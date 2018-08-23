#ifndef DATA_ROADGRAPH_HPP
#define DATA_ROADGRAPH_HPP

#include "Layer.hpp"

namespace data {

class RoadGraph : public Layer<char> {

public:
  RoadGraph(unsigned int sideLength);
};
}

#endif