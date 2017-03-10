#ifndef STATES_MAINMENUSTATE_HPP
#define STATES_MAINMENUSTATE_HPP

#include <chrono>
#include <cmath>
#include <iostream>

#include "../engine/Engine.hpp"
#include "../engine/GameState.hpp"
#include "../rendering/Renderer.hpp"
#include "../rendering/UI.hpp"
#include "MapState.hpp"

namespace states {

class MainMenuState : public engine::GameState {

public:
  MainMenuState(engine::Engine& engine, rendering::Renderer& renderer, MapState& mapState);

  void init();

  void update(std::chrono::milliseconds delta);
  void render();

  void onKey(int key, int scancode, int action, int mods);
  void onMouseButton(int button, int action, int mods);
  void onScroll(double xoffset, double yoffset);
  void onWindowFocusChange(int focused);
  void onWindowResize(int width, int height);

private:
  rendering::Renderer& renderer;

  // FIXME(kantoniak): MainMenuState::renderButton() should not be there
  static constexpr unsigned short buttonsCount = 2;
  const char* const buttons[buttonsCount] = {"Start game", "Exit"};
  void renderButton(const char* label, glm::vec2 pos, unsigned short padding);

  // FIXME(kantoniak): MainMenuState::hitTestButton() is so lame
  int hitTestButton(glm::vec2 mouse);

  MapState& mapState;
  void startGame();
};
}

#endif
