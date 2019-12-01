#include "SaveFileHandler.hpp"

#include <iostream>

namespace io {

SaveFileHandler::SaveFileHandler(engine::Engine& engine) : engine(engine) {
}

void SaveFileHandler::createSave(world::World& world) {

  engine.getLogger().info("Saving into %s...", DEFAULT_SAVE_FILE_NAME.c_str());

  auto saveFileStream = std::ofstream(DEFAULT_SAVE_FILE_NAME, std::ios::out | std::ios::binary);
  if (!saveFileStream.good()) {
    return;
  }

  // Create an output archive
  cereal::BinaryOutputArchive archive(saveFileStream);

  archive(world.getCamera().getCameraState());

  unsigned int chunksCount = world.getMap().getChunks().size();
  archive(chunksCount);

  for (data::Chunk* ptr : world.getMap().getChunks()) {
    const data::Chunk chunk = data::Chunk(*ptr);
    archive(chunk);
  }
}

void SaveFileHandler::loadSave(world::World& world) {

  engine.getLogger().info("Loading from %s...", DEFAULT_SAVE_FILE_NAME.c_str());

  auto saveFileStream = std::ifstream(DEFAULT_SAVE_FILE_NAME, std::ios::in | std::ios::binary);
  if (!saveFileStream.good()) {
    engine.getLogger().error("Could not open file for reading.");
    return;
  }
  cereal::BinaryInputArchive archive(saveFileStream); // Create an output archive

  data::CameraState cameraState;
  archive(cameraState);

  engine.getLogger().debug(to_string(cameraState));
  world.getCamera().emplace(cameraState);

  unsigned int chunksCount;
  archive(chunksCount);
  engine.getLogger().debug("Loading %d chunks:", chunksCount);

  for (unsigned int i = 0; i < chunksCount; i++) {
    engine.getLogger().debug("Loading chunk %d", i);
    data::Chunk chunk;
    archive(chunk);
    world.getMap().loadChunk(chunk);
  }

  engine.getLogger().debug("World loaded.");
}
}