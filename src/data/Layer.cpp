#include "Layer.hpp"

namespace data {

template <class T>
Layer<T>::Layer(unsigned int sideLength) : sideLength(sideLength), layerData(sideLength * sideLength, 0) {
}

template <class T> Layer<T>::Layer(const Layer& layer) : sideLength(layer.sideLength), layerData(layer.layerData) {
}

template <class T> const std::vector<T>& Layer<T>::getLayerData() const {
  return this->layerData;
}

template class Layer<char>;
}