#pragma once

#include <tuple>

#include "cse/object.hpp"
#include "cse/resource.hpp"
#include "glm/ext/vector_int3.hpp"

namespace csg
{
  class player : public cse::object
  {
  public:
    player(const std::tuple<glm::ivec3, glm::ivec3, glm::ivec3> &transform_);
  };

  class environment : public cse::object
  {
  public:
    environment(const std::tuple<glm::ivec3, glm::ivec3, glm::ivec3> &transform_, const cse::compiled_image &image_,
                const cse::compiled_frame_group &frame_group_);
  };
}
