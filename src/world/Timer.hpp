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

  bool paused();
  const std::string getDate();

protected:
  static constexpr unsigned short SPEED_1_TURN_LENGTH = 2000;
  static constexpr unsigned short SPEED_2_TURN_LENGTH = 500;
  static constexpr unsigned short SPEED_3_TURN_LENGTH = 125;

  bool isPaused;
  unsigned long turnNumber;
  unsigned short sinceTurnStart;
  unsigned short currentSpeed;

  [[nodiscard]] unsigned short getTurnLength() const;

  tm dateToShow;
};
}

#endif
