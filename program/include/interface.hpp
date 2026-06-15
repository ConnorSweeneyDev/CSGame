#pragma once

#include "SDL3/SDL_stdinc.h"
#include "cse/interface.hpp"
#include "glm/ext/vector_double2.hpp"

namespace csg
{
  class icon final : public cse::interface
  {
  public:
    icon(const glm::dvec2 &translation_);

  protected:
    void on_prepare() override final;
    void on_simulate(const double tick) override final;
    void on_hover() override final;
    void on_unhover() override final;
    void on_press(const Uint8 button) override final;
    void on_release(const Uint8 button) override final;
    void on_click(const Uint8 button) override final;
    void on_scroll(const glm::dvec2 &delta) override final;

  private:
    bool is_dragging{};
    bool is_red{};
  };
}
