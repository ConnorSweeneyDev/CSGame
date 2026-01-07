#pragma once

#include "cse/camera.hpp"
#include "glm/ext/vector_float3.hpp"

namespace csg
{
  class camera : public cse::camera
  {
  public:
    camera(const glm::vec3 &translation_);

  private:
    float max_velocity{200.0f};
    float friction{50.0f};
  };
}
