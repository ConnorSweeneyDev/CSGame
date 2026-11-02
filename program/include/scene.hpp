#pragma once

#include <memory>

#include "SDL3/SDL_events.h"
#include "cse/scene.hpp"

namespace csg
{
  class scene final : public cse::scene
  {
  public:
    scene() = default;

    static void main(const std::shared_ptr<scene> &s);
    static void other(const std::shared_ptr<scene> &s);

  protected:
    void pre_prepare() final;
    void pre_event(const SDL_Event &event) final;
  };
}
