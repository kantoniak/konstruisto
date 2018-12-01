#ifndef ENGINE_DEBUGINFO_HPP
#define ENGINE_DEBUGINFO_HPP

#include <chrono>
#include <vector>

namespace engine {

class DebugInfo {
  using Clock = std::chrono::steady_clock;
  using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

public:
  DebugInfo();

  // TODO(kantoniak): Think of more generic timing methods in DebugInfo
  void onFrameStart();
  void onRenderStart();
  void onRenderEnd();
  void onRenderWorldEnd();
  void onRenderUIEnd();
  void onFrameEnd();

  unsigned int getFPS();
  unsigned int getFrameTime();
  unsigned int getRenderTime();
  unsigned int getRenderWorldTime();
  unsigned int getRenderUITime();

protected:
  unsigned const static int AVG_SAMPLES = 10;
  unsigned int currentSample;

  std::vector<unsigned int> frameTimes;
  std::vector<unsigned int> renderTimes;
  std::vector<unsigned int> renderWorldTimes;
  std::vector<unsigned int> renderUITimes;

  TimePoint frameStart;
  TimePoint renderStart;

  unsigned int FPS;
  unsigned int frameTime;
  unsigned int renderTime;
  unsigned int renderWorldTime;
  unsigned int renderUITime;

  unsigned int millisSince(TimePoint since);
  void updateTimes();
};
}

#endif
