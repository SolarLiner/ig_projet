//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_EVENTS_H
#define IG_PROJET_EVENTS_H

#include "glm/vec2.hpp"

#include <SDL.h>

namespace shell::events {
    struct KeyboardEvent {
        SDL_Keysym key;
        bool pressed;

        [[nodiscard]] bool is(SDL_KeyCode code) const { return key.sym == code; }
        [[nodiscard]] bool is(SDL_Scancode code) const { return key.scancode == code; }

        [[nodiscard]] bool is_ctrl() const { return is_lctrl() || is_rctrl(); }
        [[nodiscard]] bool is_lctrl() const { return key.mod & SDLK_LCTRL; }
        [[nodiscard]] bool is_rctrl() const { return key.mod & SDLK_RCTRL; }

        [[nodiscard]] bool is_shift() const { return is_lshift() || is_rshift(); }
        [[nodiscard]] bool is_lshift() const { return key.mod & SDLK_LSHIFT; }
        [[nodiscard]] bool is_rshift() const { return key.mod & SDLK_RSHIFT; }

        [[nodiscard]] bool is_alt() const { return is_lalt() || is_ralt(); }
        [[nodiscard]] bool is_lalt() const { return key.mod & SDLK_LALT; }
        [[nodiscard]] bool is_ralt() const { return key.mod & SDLK_RALT; }

        [[nodiscard]] bool is_meta() const { return is_lmeta() || is_rmeta(); }
        [[nodiscard]] bool is_lmeta() const { return key.mod & SDLK_LGUI; }
        [[nodiscard]] bool is_rmeta() const { return key.mod & SDLK_RGUI; }
    };

    struct Close {};

    struct Resize {
        glm::vec2 size;
    };

    struct MouseButton {
        bool pressed;
        int button;
        glm::vec2 at;
    };

    struct MouseMove {
        glm::vec2 absolute, relative;
    };

    struct ScrollWheel {
        glm::vec2 delta;
    };
}// namespace shell::events

#endif//IG_PROJET_EVENTS_H
