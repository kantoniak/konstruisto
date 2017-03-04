#ifndef DATA_BUILDINGS_HPP
#define DATA_BUILDINGS_HPP

namespace data {
namespace buildings {

struct Building {
  unsigned int objectId;
  long x;
  long y;
  short width;
  short length;
  unsigned short level;
};
}
}

#endif