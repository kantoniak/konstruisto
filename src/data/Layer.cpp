#include "Layer.hpp"

namespace data {

template <class T, unsigned int L>
Layer<T, L>::Layer() : layerData(L * L, 0) {
}

template <class T, unsigned int L>
Layer<T, L>::Layer(const Layer& layer) : layerData(layer.layerData) {
}

template <class T, unsigned int L>
const std::vector<T>& Layer<T, L>::getLayerData() const {
  return this->layerData;
}

template class Layer<char, 64u>;
}