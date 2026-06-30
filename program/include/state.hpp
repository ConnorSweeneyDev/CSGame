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
    ENLIST(game,                    //
           (master, double, {0.5}), //
           (sound, double, {1.0}),  //
           (music, double, {1.0}));
    ENLIST(window,                                                       //
           (display, SDL_DisplayID, {PRIMARY}),                          //
           (position, (std::pair<int, int>), {ORIGIN, ORIGIN}),          //
           (size, (std::pair<unsigned int, unsigned int>), {1280, 720}), //
           (mode, ::mode, {WINDOWED}),                                   //
           (vsync, bool, {true}));

  public:
    settings() : cse::state("CSGame/settings") {}
    FIELD(settings::game, game, {});
    FIELD(settings::window, window, {});
  };
}
