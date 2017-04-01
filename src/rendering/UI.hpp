#ifndef RENDERING_UI_HPP
#define RENDERING_UI_HPP

#include <map>

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include <nanovg.h>

#include "../engine/DebugInfo.hpp"
#include "../engine/Engine.hpp"
#include "../input/Selection.hpp"
#include "../input/WindowHandler.hpp"
#include "../world/World.hpp"
#include "ShaderManager.hpp"

namespace rendering {

class UI {

public:
  UI(engine::Engine& engine);

  bool init();
  void cleanup();

  NVGcontext* getContext();
  void startFrame();
  void endFrame();

  const NVGcolor getBackgroundColor() const;
  const NVGcolor getPrimaryTextColor() const;
  const NVGcolor getAccentColor() const;

  void renderLogo(float x, float y);
  void renderIcon(unsigned int icon, float x, float y);

  static constexpr const char* FONT_SSP_REGULAR = "Source Sans Pro Regular";
  static constexpr const char* FONT_SSP_BOLD = "Source Sans Pro Bold";

  static constexpr const char* LOGO_PATH = "assets/textures/ui/logo.png";

  static constexpr const unsigned char ICON_SIDE = 16;
  static constexpr const unsigned int ICON_SPEED_0 = 1;
  static constexpr const unsigned int ICON_SPEED_1 = 2;
  static constexpr const unsigned int ICON_SPEED_2 = 3;
  static constexpr const unsigned int ICON_SPEED_3 = 4;
  static constexpr const unsigned int ICON_BUILDING = 5;
  static constexpr const unsigned int ICON_ROAD = 6;
  static constexpr const unsigned int ICON_MORE = 7;

  static constexpr const char* ICON_PATH_SPEED_0 = "assets/textures/ui/icons/speed-0.png";
  static constexpr const char* ICON_PATH_SPEED_1 = "assets/textures/ui/icons/speed-1.png";
  static constexpr const char* ICON_PATH_SPEED_2 = "assets/textures/ui/icons/speed-2.png";
  static constexpr const char* ICON_PATH_SPEED_3 = "assets/textures/ui/icons/speed-3.png";
  static constexpr const char* ICON_PATH_BUILDING = "assets/textures/ui/icons/building.png";
  static constexpr const char* ICON_PATH_ROAD = "assets/textures/ui/icons/road.png";
  static constexpr const char* ICON_PATH_MORE = "assets/textures/ui/icons/more.png";

protected:
  engine::Engine& engine;
  NVGcontext* nvgContext;

  NVGcolor bgColor;
  NVGcolor primaryTextColor;
  NVGcolor accentColor;

  static constexpr const char* FONT_SSP_REGULAR_PATH = "assets/fonts/SourceSansPro/SourceSansPro-Regular.ttf";
  static constexpr const char* FONT_SSP_BOLD_PATH = "assets/fonts/SourceSansPro/SourceSansPro-Bold.ttf";

  int logoImage;
  std::map<unsigned int, int> icons;

  bool loadIcon(unsigned int icon, const char* filename);
};
}

#endif
