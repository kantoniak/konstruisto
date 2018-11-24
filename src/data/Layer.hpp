#ifndef DATA_LAYER_HPP
#define DATA_LAYER_HPP

#include <iostream>
#include <vector>

namespace data {

template <class T, unsigned int L> class Layer {

public:
  Layer();
  Layer(const Layer& layer);
  const std::vector<T>& getLayerData() const;

protected:
  std::vector<T> layerData;
};
}

#endif
