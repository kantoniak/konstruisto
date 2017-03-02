#include "DebugInfo.hpp"

#include <iostream>

namespace engine {
DebugInfo::DebugInfo()
    : frameTimes(AVG_SAMPLES, 0), renderTimes(AVG_SAMPLES, 0), renderWorldTimes(AVG_SAMPLES, 0),
      renderUITimes(AVG_SAMPLES, 0) {
  FPS = 0;
  frameTime = 0;
  renderTime = 0;
  renderWorldTime = 0;
  renderUITime = 0;
  currentSample = 0;
}

void DebugInfo::onFrameStart() {
  frameStart = Clock::now();
}

void DebugInfo::onRenderStart() {
  renderStart = Clock::now();
}

void DebugInfo::onRenderWorldEnd() {
  renderWorldTimes[currentSample] = millisSince(renderStart);
}

void DebugInfo::onRenderUIEnd() {
  renderUITimes[currentSample] = millisSince(renderStart);
}

void DebugInfo::onRenderEnd() {
  renderTimes[currentSample] = millisSince(renderStart);
}

void DebugInfo::onFrameEnd() {
  frameTimes[currentSample] = millisSince(frameStart);
  currentSample++;
  if (currentSample == AVG_SAMPLES) {
    updateTimes();
  }
}

unsigned int DebugInfo::getFPS() {
  return FPS;
}

unsigned int DebugInfo::getFrameTime() {
  return frameTime;
}

unsigned int DebugInfo::getRenderTime() {
  return renderTime;
}

unsigned int DebugInfo::getRenderWorldTime() {
  return renderWorldTime;
}

unsigned int DebugInfo::getRenderUITime() {
  return renderUITime;
}

unsigned int DebugInfo::millisSince(TimePoint since) {
  return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - since).count();
}

void DebugInfo::updateTimes() {

  FPS = 0;
  frameTime = 0;
  renderTime = 0;
  renderWorldTime = 0;
  renderUITime = 0;

  for (unsigned int it = 0; it < AVG_SAMPLES; it++) {
    frameTime += frameTimes[it];
    renderTime += renderTimes[it];
    renderWorldTime += renderWorldTimes[it];
    renderUITime += renderUITimes[it];
  }
  frameTime /= AVG_SAMPLES;
  FPS = 1000 / frameTime;
  renderTime /= AVG_SAMPLES;
  renderWorldTime /= AVG_SAMPLES;
  renderUITime /= AVG_SAMPLES;

  currentSample = 0;
}
}