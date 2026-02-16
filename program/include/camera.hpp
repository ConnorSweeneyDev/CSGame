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
    void on_input(const bool *keys) override final;
    void on_simulate(const double tick) override final;

  private:
    double max_velocity{200.0};
    double friction{50.0};
    double fov_change{10.0};
  };
}
