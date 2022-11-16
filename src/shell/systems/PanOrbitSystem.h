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
        void before_run(const sf::Window &window, entt::registry &registry) override {
            if(!registry.ctx().contains<components::PanOrbitCamera>()) registry.ctx().emplace<PanOrbitCamera>();
            auto &dispatcher = registry.ctx().get<entt::dispatcher>();
            dispatcher.sink<events::MouseMove>().connect<&PanOrbitSystem::handle_mouse_move>(*this);
            dispatcher.sink<events::MouseButton>().connect<&PanOrbitSystem::handle_mouse_press>(*this);
            dispatcher.sink<events::ScrollWheel>().connect<&PanOrbitSystem::handle_scroll>(*this);
            auto size = window.getSize();
            window_size = vec2(size.x, size.y);
        }

        void operator()(const sf::Window &window, entt::registry &registry) override {
            auto dt = registry.ctx().get<components::Time>().last_frame.asSeconds();
            auto &camera = registry.ctx().get<gl::Camera>();
            auto &controller = registry.ctx().get<components::PanOrbitCamera>();
            auto size = window.getSize();
            window_size = vec2(size.x, size.y);
            controller.zoom(zoom_ticks * dt);
            if(left_pressed) controller.pan(relative * dt);
            if(right_pressed) controller.translate(relative * dt);
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

        void handle_scroll(events::ScrollWheel wheel) { zoom_ticks += wheel.ticks; }

        bool left_pressed = false, right_pressed = false;
        vec2 relative{0}, last_mouse_pos{0}, window_size{0};
        float zoom_ticks;
    };
}// namespace shell::systems

#endif//IG_PROJET_PANORBITSYSTEM_H
