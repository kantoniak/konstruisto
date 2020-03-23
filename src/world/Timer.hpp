#ifndef WORLD_TIMER_HPP
#define WORLD_TIMER_HPP

#include <chrono>
#include <ctime>

#include "../engine/Engine.hpp"

namespace world {

class Timer {

public:
  void init();
  void update(std::chrono::milliseconds delta);

  void pause();
  void start();

  void setSpeed(unsigned short speed);
  unsigned short getSpeed();
  constexpr unsigned short getMaxSpeed() {
    return 3;
  }

  [[nodiscard]] double get_turn_number() const noexcept;
  [[nodiscard]] float delta_to_turns(std::chrono::milliseconds delta) const noexcept;

  bool paused();
  const std::string getDate();

protected:
  const std::chrono::milliseconds SPEED_1_TURN_LENGTH = std::chrono::milliseconds(2000);
  const std::chrono::milliseconds SPEED_2_TURN_LENGTH = std::chrono::milliseconds(500);
  const std::chrono::milliseconds SPEED_3_TURN_LENGTH = std::chrono::milliseconds(125);

  bool isPaused;
  unsigned long turnNumber;
  std::chrono::milliseconds sinceTurnStart;
  unsigned short currentSpeed;

  [[nodiscard]] std::chrono::milliseconds getTurnLength() const;

  tm dateToShow;
};
}

#endif
