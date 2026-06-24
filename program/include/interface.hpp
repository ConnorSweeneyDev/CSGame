#pragma once

#include "cse/interface.hpp"
#include "glm/ext/vector_double2.hpp"

namespace csg
{
  class cursor final : public cse::interface
  {
  public:
    cursor();

  protected:
    void on_simulate(const double tick) override final;
  };

  class text final : public cse::interface
  {
  public:
    text(const glm::dvec2 &translation_, const glm::dvec2 &scale_);
  };

  class button final : public cse::interface
  {
  public:
    button(const glm::dvec2 &translation_);

  protected:
    void on_prepare() override final;
    void on_simulate(const double tick) override final;
  };
}
