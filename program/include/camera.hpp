#pragma once

#include <tuple>

#include "cse/camera.hpp"
#include "glm/ext/vector_float3.hpp"

namespace csg
{
  class custom_camera : public cse::core::camera
  {
  public:
    custom_camera(const std::tuple<glm::vec3, glm::vec3, glm::vec3> &transform_);
  };
}
