#pragma once

#include "cse/camera.hpp"
#include "glm/ext/vector_double3.hpp"

namespace csg
{
  class camera : public cse::camera
  {
  public:
    camera(const glm::dvec3 &translation_);

  private:
    double max_velocity{200.0};
    double friction{50.0};
    double fov_change{10.0};
  };
}
