#include "roads.hpp"

namespace data {
namespace roads {

const RoadDefinition getDefinition(Road road) {
  if (road.type == Standard.typeId) {
    return Standard;
  }
  return RoadDefinition{};
}
}
}
