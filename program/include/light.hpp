#pragma once

#include "SDL3/SDL_events.h"
#include "cse/light.hpp"
#include "glm/ext/vector_double3.hpp"

class sun final : public cse::light
{
public:
  sun();
};

class lamp final : public cse::light
{
public:
  lamp(const glm::dvec3 &translation_);

private:
  void on_event(const SDL_Event &event) override final;
};

class spot final : public cse::light
{
public:
  spot(const glm::dvec3 &translation_);
};
