#include "Timer.hpp"

#include <sstream>

namespace world {
void Timer::init() {
  isPaused = true;
  turnNumber = 0;
  sinceTurnStart = std::chrono::milliseconds(0);
  currentSpeed = 3;

  time_t t = time(nullptr);
#ifdef _WIN32
  gmtime_s(&dateToShow, &t);
#else
  gmtime_r(&t, &dateToShow);
#endif
  dateToShow.tm_year -= 30;
  dateToShow.tm_mon = 1;
  dateToShow.tm_mday = 1;
}

void Timer::update(std::chrono::milliseconds delta) {
  if (isPaused) {
    return;
  }
  sinceTurnStart += delta;
  while (sinceTurnStart >= getTurnLength()) {
    turnNumber++;
    sinceTurnStart -= getTurnLength();
    dateToShow.tm_mday += 7;
    if (turnNumber % 4 == 0) {
      dateToShow.tm_mday = 1;
      dateToShow.tm_mon++;
      while (dateToShow.tm_mon > 12) {
        dateToShow.tm_mon -= 12;
        dateToShow.tm_year++;
      }
    }
  }
}

void Timer::pause() {
  isPaused = true;
}

void Timer::start() {
  isPaused = false;
}

bool Timer::paused() {
  return isPaused;
}

const std::string Timer::getDate() {
  std::stringstream result;
  result << std::to_string(1900 + dateToShow.tm_year) << "-";
  result << std::setw(2) << std::setfill('0') << std::to_string(dateToShow.tm_mon) << "-";
  result << std::setw(2) << std::setfill('0') << std::to_string(dateToShow.tm_mday);
  return result.str();
}

void Timer::setSpeed(unsigned short speed) {
  float currentTurnPassed = sinceTurnStart / getTurnLength();
  currentSpeed = speed;
  sinceTurnStart = std::chrono::milliseconds(static_cast<long>(currentTurnPassed * getTurnLength().count()));
}

unsigned short Timer::getSpeed() {
  return currentSpeed;
}

double Timer::get_turn_number() const noexcept {
  return static_cast<double>(turnNumber) + delta_to_turns(sinceTurnStart);
}

float Timer::delta_to_turns(std::chrono::milliseconds delta) const noexcept {
  if (isPaused) {
    return 0;
  }
  return delta.count() / static_cast<double>(getTurnLength().count());
}

std::chrono::milliseconds Timer::getTurnLength() const {
  switch (currentSpeed) {
  default:
    return std::chrono::milliseconds(1);
  case 1:
    return Timer::SPEED_1_TURN_LENGTH;
  case 2:
    return Timer::SPEED_2_TURN_LENGTH;
  case 3:
    return Timer::SPEED_3_TURN_LENGTH;
  }
}
}
