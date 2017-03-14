#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "rendering/settings.hpp"
#include "world/settings.hpp"

struct settings {
  rendering::settings rendering;
  world::settings world;
};

#endif
