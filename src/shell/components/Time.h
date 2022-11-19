//
// Created by solarliner on 16/11/22.
//

#ifndef IG_PROJET_TIME_H
#define IG_PROJET_TIME_H

#include <chrono>
#include <SDL.h>

namespace shell::components {

    struct Time {
        typedef std::chrono::duration<uint64_t, std::ratio<1, 1000>> tick_t;
        typedef std::chrono::duration<float> s;
        static tick_t now();

        [[nodiscard]] tick_t since_start() const;

        [[nodiscard]] s since_start_secs() const;

        [[nodiscard]] tick_t since_frame() const;

        [[nodiscard]] s since_frame_secs() const;

        [[nodiscard]] s last_frame_secs() const;

        tick_t start, frame;
        tick_t last_frame;
    };

}// namespace shell::components

#endif//IG_PROJET_TIME_H
