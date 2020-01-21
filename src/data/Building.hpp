#ifndef DATA_BUILDING_HPP
#define DATA_BUILDING_HPP

#include "../geometry/Collidable.hpp"

namespace data {

struct Building {
  using ptr = std::shared_ptr<Building>;

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

template <typename Archive>
void save(Archive& ar, const Building::ptr& building_ptr) {
  ar(*building_ptr);
}

template <class Archive>
void load(Archive& ar, Building::ptr& building_ptr) {
  building_ptr = std::make_shared<Building>();
  ar(*building_ptr);
}

}

#endif