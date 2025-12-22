#pragma once

#include <tuple>

#include "cse/object.hpp"
#include "glm/ext/vector_int3.hpp"

namespace csg
{
  class custom_object : public cse::core::object
  {
  public:
    custom_object(const std::tuple<glm::ivec3, glm::ivec3, glm::ivec3> &transform_);
  };
}
