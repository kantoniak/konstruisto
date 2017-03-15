#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "input/settings.hpp"
#include "rendering/settings.hpp"
#include "world/settings.hpp"

struct settings {
  input::settings input;
  rendering::settings rendering;
  world::settings world;
};

#endif
