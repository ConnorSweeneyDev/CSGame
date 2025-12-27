#include "scene.hpp"

#include <memory>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/scene.hpp"

#include "camera.hpp"
#include "object.hpp"
#include "resource.hpp"

namespace csg
{
  custom_scene::custom_scene() : cse::scene()
  {
    hooks.add("pre_event",
              [this](const SDL_Event &event)
              {
                if (event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP) return;
                switch (const auto &key{event.key}; key.scancode)
                {
                  case SDL_SCANCODE_6:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                      if (auto game{parent.lock()}) game->set_current_scene("main");
                    break;
                  case SDL_SCANCODE_7:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                      if (auto game{parent.lock()})
                        game->set_current_scene(
                          "other",
                          [](const std::shared_ptr<csg::custom_scene> scene)
                          {
                            scene->set_camera<csg::custom_camera>(
                              {{0.0f, 0.0f, 80.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}});
                            scene->set_object<csg::player>("player", {{0, 0, 0}, {0, 0, 0}, {1, 1, 1}});
                            scene->set_object<csg::environment>("floor", {{0, -61, 0}, {0, 0, 0}, {1, 1, 1}},
                                                                csg::texture::floor.image,
                                                                csg::texture::floor.group.main);
                          });
                    break;
                  case SDL_SCANCODE_8:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN) camera->state.translation.value.x += 1.0f;
                    break;
                  case SDL_SCANCODE_9:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                      objects.at("player")->graphics.color = {64, 0, 0, 128};
                    else if (key.type == SDL_EVENT_KEY_UP)
                      objects.at("player")->graphics.color = {128, 128, 128, 0};
                    break;
                  default: break;
                }
              });
  }
}
