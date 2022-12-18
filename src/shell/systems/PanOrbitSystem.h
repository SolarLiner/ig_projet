//
// Created by solarliner on 15/11/22.
//

#ifndef IG_PROJET_PANORBITSYSTEM_H
#define IG_PROJET_PANORBITSYSTEM_H

#include "../components/PanOrbitCamera.h"
#include "../components/Time.h"
#include "../events.h"
#include "../gl/Renderer.h"
#include "System.h"
#include <imgui.h>

namespace shell::systems {
    using components::PanOrbitCamera;
    using glm::mat4;
    using glm::vec2;
    using glm::vec3;
    using glm::vec4;

    class PanOrbitSystem : public System {
    public:
        void before_run(Shell &shell) override;

        void execute(Shell &shell) override;

    private:
        void handle_mouse_press(events::MouseButton button);

        void handle_mouse_move(events::MouseMove move);

        void handle_scroll(events::ScrollWheel wheel);

        void handle_keys(events::KeyboardEvent event);

        bool left_pressed = false, right_pressed = false;
        vec2 relative{0}, window_size{0};
        float zoom_ticks;
    };
}// namespace shell::systems

#endif//IG_PROJET_PANORBITSYSTEM_H
