//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_EVENTS_H
#define IG_PROJET_EVENTS_H

#include <SFML/Window.hpp>

namespace shell::events {
    struct KeyboardEvent {
        sf::Keyboard::Key key;
        bool pressed;
    };

    struct Close {};

    struct Resize {
        unsigned int width;
        unsigned int height;
    };
}

#endif//IG_PROJET_EVENTS_H
