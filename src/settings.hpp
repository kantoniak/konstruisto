#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "input/settings.hpp"
#include "rendering/settings.hpp"
#include "world/settings.hpp"

struct settings {
  /// Should render single frame and stop the engine. Used for debugging
  /// leaking memory.
  bool render_frame_and_close = false;

  input::settings input;
  rendering::settings rendering;
  world::settings world;
};

#endif
