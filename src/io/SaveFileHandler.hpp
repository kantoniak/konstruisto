#ifndef IO_SAVEFILEHANDLER_HPP
#define IO_SAVEFILEHANDLER_HPP

#include <fstream>
#include <string>

#include <cereal/archives/binary.hpp>

#include "../data/CameraState.hpp"
#include "../engine/Engine.hpp"
#include "../world/World.hpp"
#include "serializers.hpp"

namespace io {

class SaveFileHandler {

public:
  const std::string DEFAULT_SAVE_FILE_NAME = "save.dat";

  SaveFileHandler(engine::Engine& engine);
  void createSave(world::World& world);
  void loadSave(world::World& world);

protected:
  engine::Engine& engine;
};
}

#endif
