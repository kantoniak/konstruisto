#ifndef DATA_CITY_HPP
#define DATA_CITY_HPP

#include <glm/glm.hpp>
#include <string>

namespace data {
struct City {
  std::string name;
  unsigned int people;
  long money;
};
}

#endif