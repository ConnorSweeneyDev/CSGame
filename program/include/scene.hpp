#pragma once

#include <memory>

#include "cse/scene.hpp"

namespace csg
{
  class scene : public cse::scene
  {
  public:
    scene();

    static void main(const std::shared_ptr<scene> main);
    static void other(const std::shared_ptr<scene> other);
  };
}
