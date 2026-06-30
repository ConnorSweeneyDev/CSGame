#pragma once

#include <utility>

#include "SDL3/SDL_video.h"
#include "cse/window.hpp"

#include "cse/state.hpp"

namespace csg
{
  class settings final : public cse::state
  {
  private:
#define GAME(X)                                                                                                        \
  X(master, double, {0.5})                                                                                             \
  X(sound, double, {1.0})                                                                                              \
  X(music, double, {1.0})
    ENLIST(game, GAME);
#undef GAME
#define WINDOW(X)                                                                                                      \
  X(display, SDL_DisplayID, {PRIMARY})                                                                                 \
  X(position, (std::pair<int, int>), {ORIGIN, ORIGIN})                                                                 \
  X(size, (std::pair<unsigned int, unsigned int>), {1280, 720})                                                        \
  X(mode, ::mode, {WINDOWED})                                                                                          \
  X(vsync, bool, {true})
    ENLIST(window, WINDOW);
#undef WINDOW

  public:
    settings() : cse::state("CSGame/settings") {}
    FIELD(settings::game, game, {});
    FIELD(settings::window, window, {});
  };
}
