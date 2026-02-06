#pragma once

#include "cse/object.hpp"
#include "cse/resource.hpp"
#include "glm/ext/vector_int3.hpp"

namespace csg
{
  class player : public cse::object
  {
  public:
    player(const glm::ivec3 &translation_);

  private:
    float max_velocity{200.0f};
    float friction{50.0f};
    double transparency_change{0.5};
  };

  class environment : public cse::object
  {
  public:
    environment(const glm::ivec3 &translation_, const cse::image &image_, const cse::group &group_);
  };
}
