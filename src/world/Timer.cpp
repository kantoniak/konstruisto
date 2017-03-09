#include "Timer.hpp"

namespace world {
void Timer::init() {
  isPaused = true;
  turnNumber = 0;
  sinceTurnStart = 0;
  currentSpeed = 3;

  time_t t = time(NULL);
  dateToShow = *(gmtime(&t));
  dateToShow.tm_year -= 30;
  dateToShow.tm_mon = 1;
  dateToShow.tm_mday = 1;
}

void Timer::update(std::chrono::milliseconds delta) {
  if (isPaused) {
    return;
  }
  sinceTurnStart += delta.count();
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

unsigned short Timer::getCurrentSpeed() {
  return currentSpeed;
}

unsigned short Timer::getTurnLength() const {
  switch (currentSpeed) {
  default:
    return 1;
  case 1:
    return Timer::SPEED_1_TURN_LENGTH;
  case 2:
    return Timer::SPEED_2_TURN_LENGTH;
  case 3:
    return Timer::SPEED_3_TURN_LENGTH;
  }
}
}