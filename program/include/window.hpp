#pragma once

#include "cse/window.hpp"

#include "SDL3/SDL_events.h"

namespace csg
{
  class window final : public cse::window
  {
  public:
    window();

  protected:
    void on_simulate(const double delta) override final;
    void on_event(const SDL_Event &event) override final;
  };
}
