#include "scene.hpp"

#include <algorithm>
#include <memory>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/game.hpp"
#include "cse/resource.hpp"
#include "cse/scene.hpp"

#include "resource.hpp"

namespace csg
{
  void scene::pre_prepare()
  {
    auto &song = active.mixer.set("main", music::main);
    song.playing = true;
    song.speed.value = 0.6;
    song.loop = true;
    song.volume.value = volume;
  }

  void scene::pre_event(const SDL_Event &event)
  {
    if (event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP) return;
    switch (const auto &key{event.key}; key.scancode)
    {
      case SDL_SCANCODE_N:
        if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
        {
          auto &song{active.mixer.get<cse::music>("main")};
          if (active.timer.has("fade_out"))
            fade_in(duration - active.timer.get("fade_out").elapsed);
          else if (active.timer.has("fade_in"))
            fade_out(duration - active.timer.get("fade_in").elapsed);
          else if (song.playing)
          {
            if (song.volume.value > 0.0) volume = song.volume.value;
            fade_out(0.0);
          }
          else
          {
            if (song.volume.value > 0.0) volume = song.volume.value;
            song.playing = true;
            fade_in(0.0);
          }
        }
        break;
      case SDL_SCANCODE_6:
        if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN) game->current("main");
        break;
      case SDL_SCANCODE_7:
        if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN) game->current("other", other);
        break;
      case SDL_SCANCODE_8:
        if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN) active.mixer.remove<cse::music>("main");
        break;
      case SDL_SCANCODE_9:
        if (const auto &player{active.objects.find("player")})
        {
          if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
            player->active.texture.color.tint.value = {0.25, 0.0, 0.0, 0.5};
          else if (key.type == SDL_EVENT_KEY_UP)
            player->active.texture.color.tint.value = {0.5, 0.5, 0.5, 1.0};
        }
        break;
      default: break;
    }
  }

  void scene::pre_simulate(const double)
  {
    if (active.mixer.has<cse::music>("main"))
    {
      auto &target{active.mixer.get<cse::music>("main").volume.value};
      if (active.timer.has("fade_out"))
      {
        const auto &state{active.timer.get("fade_out")};
        target = volume * std::max(0.0, 1.0 - (state.elapsed / state.target));
      }
      else if (active.timer.has("fade_in"))
      {
        const auto &state{active.timer.get("fade_in")};
        target = volume * std::min(1.0, state.elapsed / state.target);
      }
    }
    active.timer.call<void()>("fade_out");
    active.timer.call<void()>("fade_in");
  }

  void scene::fade_out(const double elapsed)
  {
    active.timer.remove("fade_in");
    auto &state{active.timer.set("fade_out",
                                 [this]()
                                 {
                                   if (!active.mixer.has<cse::music>("main")) return;
                                   auto &track{active.mixer.get<cse::music>("main")};
                                   track.volume.value = 0.0;
                                   track.playing = false;
                                 })};
    state.target = duration;
    state.elapsed = elapsed;
  }

  void scene::fade_in(const double elapsed)
  {
    active.timer.remove("fade_out");
    auto &state{active.timer.set("fade_in",
                                 [this]()
                                 {
                                   if (active.mixer.has<cse::music>("main"))
                                     active.mixer.get<cse::music>("main").volume.value = volume;
                                 })};
    state.target = duration;
    state.elapsed = elapsed;
  }
}
