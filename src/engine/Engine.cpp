#include "Engine.hpp"

#include <cassert>

namespace engine {

Engine::Engine(Logger& logger) : isRunning(true), logger(logger) {
}

Engine::~Engine() {
}

bool Engine::running() {
  return isRunning;
}

void Engine::stop() {
  isRunning = false;
}

bool Engine::init(rendering::WindowHandler& windowHandler) {
  srand(time(nullptr));

  this->start = std::chrono::high_resolution_clock::now();
  this->current = this->start;

  this->windowHandler = &windowHandler;

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

GameState& Engine::getPreviousState() {
  return *(states.at(states.size() - 2));
}

void Engine::tick(std::chrono::time_point<std::chrono::high_resolution_clock> now) {
  auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - current);
  if (delta < std::chrono::milliseconds(16))
    return;

  this->current = now;
  this->update(delta);
  this->render();
}

std::chrono::milliseconds Engine::getDeltaSinceStart() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(current - start);
}

Logger& Engine::getLogger() const {
  return logger;
}

rendering::WindowHandler& Engine::getWindowHandler() const {
  assert(windowHandler != nullptr);
  return *windowHandler;
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
