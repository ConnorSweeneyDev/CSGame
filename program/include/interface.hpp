#pragma once

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
  };
}
