//
// Created by solarliner on 16/11/22.
//

#ifndef IG_PROJET_TIME_H
#define IG_PROJET_TIME_H

#include "SFML/System/Clock.hpp"

namespace shell::components {

    struct Time {
        [[nodiscard]] sf::Time total_elapsed() const { return start.getElapsedTime(); }
        [[nodiscard]] sf::Time frame_elapsed() const { return frame.getElapsedTime(); }
        sf::Clock start, frame;
        sf::Time last_frame;
    };

}// namespace shell::components

#endif//IG_PROJET_TIME_H
