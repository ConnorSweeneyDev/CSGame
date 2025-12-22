#pragma once

#include <tuple>

#include "cse/object.hpp"
#include "cse/resource.hpp"
#include "glm/ext/vector_int3.hpp"

namespace csg
{
  class player : public cse::core::object
  {
  public:
    player(const std::tuple<glm::ivec3, glm::ivec3, glm::ivec3> &transform_);
  };

  class environment : public cse::core::object
  {
  public:
    environment(const std::tuple<glm::ivec3, glm::ivec3, glm::ivec3> &transform_,
                const cse::resource::compiled_texture &texture_);
  };
}
