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
  MainMenuState(engine::Engine& engine, MapState& mapState);

  void init() override;

  void update(std::chrono::milliseconds delta) override;
  void render() override;

  void onKey(int key, int scancode, int action, int mods) override;
  void onMouseButton(int button, int action, int mods) override;
  void onScroll(double xoffset, double yoffset) override;
  void onWindowFocusChange(int focused) override;
  void onWindowResize(int width, int height) override;

private:
  // FIXME(kantoniak): MainMenuState::renderButton() should not be there
  static constexpr unsigned short buttonsCount = 2;
  const std::array<const char*, buttonsCount> buttons = {"Start game", "Exit"};
  void renderButton(const char* label, glm::vec2 pos, unsigned short padding);

  // FIXME(kantoniak): MainMenuState::hitTestButton() is so lame
  int hitTestButton(glm::vec2 mouse);

  MapState& mapState;
  void startGame();
};
}

#endif
