//
// Created by solarliner on 19/11/22.
//
#include "Time.h"

namespace shell::components {
    std::chrono::duration<uint64_t, std::ratio<1, 1000>> shell::components::Time::now() {
        return (tick_t) SDL_GetTicks64();
    }
    std::chrono::duration<uint64_t, std::ratio<1, 1000>> shell::components::Time::since_start() const {
        return now() - start;
    }
    Time::s Time::since_start_secs() const {
        return duration_cast<s>(since_start());
    }
    Time::tick_t Time::since_frame() const {
        return now() - frame;
    }
    Time::s Time::since_frame_secs() const {
        return duration_cast<s>(since_frame());
    }
    Time::s Time::last_frame_secs() const {
        return duration_cast<s>(last_frame);
    }
}