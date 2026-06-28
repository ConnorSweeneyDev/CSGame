#pragma once

#include "cse/camera.hpp"
#include "glm/ext/vector_double3.hpp"

namespace csg
{
  class camera final : public cse::camera
  {
  public:
    camera(const glm::dvec3 &translation_);

  protected:
    void on_simulate(const double tick) override final;

  private:
    static constexpr double max_velocity{200.0};
    static constexpr double friction{50.0};
    static constexpr double forward_change{1.5};
  };
}
