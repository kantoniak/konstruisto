#include "Engine.hpp"

#include <cassert>

namespace engine {

Engine::Engine(settings& gameSettings, Logger& logger) : gameSettings(gameSettings), isRunning(true), logger(logger) {
}

Engine::~Engine() {
}

bool Engine::running() {
  return isRunning;
}

void Engine::stop() {
  isRunning = false;
}

bool Engine::init(input::WindowHandler& windowHandler, rendering::UI& ui) {
  srand(time(nullptr));

  this->start = std::chrono::high_resolution_clock::now();
  this->current = this->start;

  this->windowHandler = &windowHandler;
  this->ui = &ui;

  return true;
}

void Engine::cleanup() {
  while (!states.empty()) {
    states.back()->cleanup();
    states.pop_back();
  }
}

void Engine::changeState(GameState& state) {

  if (!states.empty()) {
    states.back()->cleanup();
    states.pop_back();
  }

  states.push_back(&state);
  states.back()->init();
}

void Engine::pushState(GameState& state) {

  if (!states.empty())
    states.back()->suspend();

  states.push_back(&state);
  states.back()->init();
}

void Engine::popState() {

  if (!states.empty()) {
    states.back()->cleanup();
    states.pop_back();
  }

  if (!states.empty())
    states.back()->unsuspend();
}

GameState* Engine::getCurrentState() {
  if (states.empty()) {
    return nullptr;
  }
  return states.at(states.size() - 1);
}

GameState* Engine::getPreviousState() {
  if (states.size() < 2) {
    return nullptr;
  }
  return states.at(states.size() - 2);
}

void Engine::tick(std::chrono::time_point<std::chrono::high_resolution_clock> now) {
  auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - current);

  this->current = now;
  this->update(delta);

  getDebugInfo().onRenderStart();
  this->render();
  getDebugInfo().onRenderEnd();
}

std::chrono::milliseconds Engine::getDeltaSinceStart() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(current - start);
}

settings& Engine::getSettings() const {
  return gameSettings;
}

Logger& Engine::getLogger() const {
  return logger;
}

DebugInfo& Engine::getDebugInfo() {
  return debugInfo;
}

input::WindowHandler& Engine::getWindowHandler() const {
  assert(windowHandler != nullptr);
  return *windowHandler;
}

rendering::UI& Engine::getUI() const {
  assert(ui != nullptr);
  return *ui;
}

void Engine::update(std::chrono::milliseconds delta) {
  if (!states.empty() && !states.back()->isSuspended())
    states.back()->update(delta);
}

void Engine::render() {
  if (!states.empty())
    states.back()->render();
}
}
