#pragma once

#include <tuple>

#include "cse/camera.hpp"
#include "glm/ext/vector_float3.hpp"

namespace csg
{
  class camera : public cse::camera
  {
  public:
    camera(const std::tuple<glm::vec3, glm::vec3, glm::vec3> &transform_);
  };
}
