#ifndef DATA_LAYER_HPP
#define DATA_LAYER_HPP

#include <iostream>
#include <vector>

namespace data {

template <class T> class Layer {

public:
  Layer(unsigned int sideLength);
  Layer(const Layer & layer);
  const std::vector<T>& getLayerData() const;

protected:
  unsigned int sideLength;
  std::vector<T> layerData;
};
}

#endif
