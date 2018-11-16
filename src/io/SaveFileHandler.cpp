#include "SaveFileHandler.hpp"

#include <iostream>

namespace io {

SaveFileHandler::SaveFileHandler(engine::Engine& engine) : engine(engine) {
}

void SaveFileHandler::createSave(world::World& world) {

  engine.getLogger().info("Saving into %s...", DEFAULT_SAVE_FILE_NAME.c_str());

  {
    auto saveFileStream = std::ofstream(DEFAULT_SAVE_FILE_NAME, std::ios::out | std::ios::binary);
    if (saveFileStream.good()) {
      cereal::BinaryOutputArchive archive(saveFileStream); // Create an output archive
      archive(world.getCamera().getCameraState());
    }
  }
}

void SaveFileHandler::loadSave(world::World& world) {

  engine.getLogger().info("Loading from %s...", DEFAULT_SAVE_FILE_NAME.c_str());

  {
    auto saveFileStream = std::ifstream(DEFAULT_SAVE_FILE_NAME, std::ios::out | std::ios::binary);
    if (saveFileStream.good()) {
      cereal::BinaryInputArchive archive(saveFileStream); // Create an output archive

      data::CameraState cameraState;
      archive(cameraState);

      engine.getLogger().debug("Camera lookAt[%f][%f][%f], rot[%f][%f], dist[%f] ", cameraState.lookAt.x,
                               cameraState.lookAt.y, cameraState.lookAt.z, cameraState.rotationAroundX,
                               cameraState.rotationAroundY, cameraState.distance);
      world.getCamera().emplace(cameraState);
    }
  }
}
}