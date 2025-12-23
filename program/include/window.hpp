#pragma once

#include <string>

#include "cse/window.hpp"
#include "glm/ext/vector_uint2.hpp"

namespace csg
{
  class custom_window : public cse::window
  {
  public:
    custom_window(const std::string &title_, const glm::uvec2 &dimensions_);
  };
}
