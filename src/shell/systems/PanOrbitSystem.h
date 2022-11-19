//
// Created by solarliner on 15/11/22.
//

#ifndef IG_PROJET_PANORBITSYSTEM_H
#define IG_PROJET_PANORBITSYSTEM_H

#include "../components/PanOrbitCamera.h"
#include "../components/Time.h"
#include "../events.h"
#include "System.h"

namespace shell::systems {
    using components::PanOrbitCamera;
    using glm::mat4;
    using glm::vec2;
    using glm::vec3;
    using glm::vec4;

    class PanOrbitSystem : public System {
    public:
        void before_run(Shell &shell) override {
            auto &res = shell.resources();
            if (!res.contains<components::PanOrbitCamera>()) res.emplace<PanOrbitCamera>();
            auto &dispatcher = res.get<entt::dispatcher>();
            dispatcher.sink<events::MouseMove>().connect<&PanOrbitSystem::handle_mouse_move>(*this);
            dispatcher.sink<events::MouseButton>().connect<&PanOrbitSystem::handle_mouse_press>(*this);
            dispatcher.sink<events::ScrollWheel>().connect<&PanOrbitSystem::handle_scroll>(*this);
            window_size = shell.window_size();
        }

        void execute(Shell &shell) override {
            auto &time = shell.resources().get<components::Time>();
            auto dt = time.last_frame_secs().count();
            auto &camera = shell.resources().get<gl::Camera>();
            auto &controller = shell.resources().get<components::PanOrbitCamera>();
            window_size = shell.window_size();
            controller.zoom(zoom_ticks * dt);
            if (left_pressed) controller.pan(relative * dt);
            if (right_pressed) controller.translate(relative * dt);
            zoom_ticks = 0;
            relative *= 0;
            controller.update(camera);
        }

    private:
        void handle_mouse_press(events::MouseButton button) {
            switch (button.button) {
                case 0:
                    left_pressed = button.pressed;
                    break;
                case 1:
                    right_pressed = button.pressed;
                    break;
                default:
                    break;
            }
        }

        void handle_mouse_move(events::MouseMove move) {
            relative += (move.absolute - last_mouse_pos) / window_size;
            last_mouse_pos = move.absolute;
        }

        void handle_scroll(events::ScrollWheel wheel) { zoom_ticks += wheel.delta.y; }

        bool left_pressed = false, right_pressed = false;
        vec2 relative{0}, last_mouse_pos{0}, window_size{0};
        float zoom_ticks;
    };
}// namespace shell::systems

#endif//IG_PROJET_PANORBITSYSTEM_H
