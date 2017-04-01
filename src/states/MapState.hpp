#ifndef STATES_MAPSTATE_HPP
#define STATES_MAPSTATE_HPP

#include <chrono>
#include <cmath>
#include <iostream>

#include "../data/CameraState.hpp"
#include "../data/City.hpp"
#include "../engine/Engine.hpp"
#include "../engine/GameState.hpp"
#include "../input/Selection.hpp"
#include "../input/WindowHandler.hpp"
#include "../rendering/Renderer.hpp"
#include "../settings.hpp"
#include "../world/Camera.hpp"
#include "../world/Geometry.hpp"
#include "../world/World.hpp"
#include "MapPauseState.hpp"

namespace states {

enum MapStateAction { PLACE_BUILDING = 0, PLACE_ROAD = 1 };

class MapState : public engine::GameState {

public:
  MapState(engine::Engine& engine);

  void init();
  void cleanup();

  void update(std::chrono::milliseconds delta);
  void render();

  void onKey(int key, int scancode, int action, int mods);
  void onMouseButton(int button, int action, int mods);
  void onScroll(double xoffset, double yoffset);
  void onWindowFocusChange(int focused);
  void onWindowResize(int width, int height);

private:
  rendering::WorldRenderer renderer;
  world::Geometry geometry;
  world::World world;
  std::unique_ptr<input::Selection> selection;

  void createRandomWorld();
  data::City city;

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
