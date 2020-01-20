#ifndef STATES_MAPSTATE_HPP
#define STATES_MAPSTATE_HPP

#include <chrono>
#include <cmath>
#include <iostream>

#include "../data/CameraState.hpp"
#include "../data/City.hpp"
#include "../data/Road.hpp"
#include "../engine/Engine.hpp"
#include "../engine/GameState.hpp"
#include "../input/Brush.hpp"
#include "../input/LineSelection.hpp"
#include "../input/Selection.hpp"
#include "../input/WindowHandler.hpp"
#include "../io/SaveFileHandler.hpp"
#include "../rendering/Renderer.hpp"
#include "../settings.hpp"
#include "../world/Camera.hpp"
#include "../world/Geometry.hpp"
#include "../world/World.hpp"
#include "MapPauseState.hpp"

namespace states {

enum MapStateAction { PLACE_BUILDING = 0, PLACE_ZONE = 1, PLACE_ROAD = 2, PLACE_TREES = 3, BULDOZE = 4 };

class MapState : public engine::GameState {

public:
  explicit MapState(engine::Engine& engine);

  void init() override;
  void cleanup() override;

  void update(std::chrono::milliseconds delta) override;
  void render() override;

  void onKey(int key, int scancode, int action, int mods) override;
  void onMouseButton(int button, int action, int mods) override;
  void onScroll(double xoffset, double yoffset) override;
  void onWindowFocusChange(int focused) override;
  void onWindowResize(int width, int height) override;

private:
  const float TREE_BRUSH_RADIUS_SINGLE = 0.1f;
  const float TREE_BRUSH_RADIUS_SMALL = 1.f;
  const float TREE_BRUSH_RADIUS_NORMAL = 3.f;
  const float TREE_BRUSH_RADIUS_BIG = 8.f;
  const float TREE_BRUSH_BORDER_WIDTH = 0.05f;

  rendering::WorldRenderer renderer;
  world::Geometry geometry;
  world::World world;

  std::unique_ptr<input::Selection> selection;
  std::shared_ptr<input::Brush> current_brush;

  // Tree painting
  std::shared_ptr<input::Brush> tree_brush;
  static data::Tree create_random_tree(const data::Position<float>& position) noexcept;

  io::SaveFileHandler saveFileHandler;
  data::City city;
  void createNewWorld();
  void createRandomWorld();
  void init_brushes() noexcept;

  MapStateAction currentAction;
  void setCurrentAction(MapStateAction action);

  // TODO(kantoniak): Move actions to InputHandler
  glm::vec2 dragStart = glm::vec2();
  bool rmbPressed = false;
  bool rotatingClockwise = false;
  bool rotatingCounterClockwise = false;
  bool rotatingUpwards = false;
  bool rotatingDownwards = false;

  void handleMapDragging(std::chrono::milliseconds delta);
  void handleRotatingAroundY(std::chrono::milliseconds delta, bool clockwise);
  void handleRotatingAroundX(std::chrono::milliseconds delta, bool upwards);

  short newBuildingHeight = 1;

  // Pause screen
  MapPauseState pauseState;
  void switchToPauseState();
};
}

#endif
