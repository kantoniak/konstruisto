#include "Engine.hpp"

namespace engine {

Engine::~Engine() {
}

bool Engine::init() {
  this->start = std::chrono::high_resolution_clock::now();
  this->current = this->start;
  return true;
}

void Engine::cleanup() {
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

void Engine::update(std::chrono::milliseconds delta) {
  if (!states.empty() && !states.back()->isSuspended())
    states.back()->update(delta);
}

void Engine::render() {
  if (!states.empty())
    states.back()->render();
}
}
