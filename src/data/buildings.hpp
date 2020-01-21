#ifndef DATA_BUILDINGS_HPP
#define DATA_BUILDINGS_HPP

#include "../geometry/Collidable.hpp"

namespace data::buildings {

struct Building {
  // unsigned int objectId;
  long x;
  long y;
  unsigned short width;
  unsigned short length;
  unsigned short level;
  geometry::Collidable::ptr body;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(x, y);
    archive(width, length, level);
  }
};
}

#endif