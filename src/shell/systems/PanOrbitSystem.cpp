//
// Created by solarliner on 21/11/22.
//

#include "PanOrbitSystem.h"

namespace shell::systems {

    void PanOrbitSystem::before_run(Shell &shell) {
        auto &res = shell.resources();
        if (!res.contains<components::PanOrbitCamera>()) res.emplace<PanOrbitCamera>();
        auto &dispatcher = res.get<entt::dispatcher>();
        dispatcher.sink<events::KeyboardEvent>().connect<&PanOrbitSystem::handle_keys>(*this);
        dispatcher.sink<events::MouseMove>().connect<&PanOrbitSystem::handle_mouse_move>(*this);
        dispatcher.sink<events::MouseButton>().connect<&PanOrbitSystem::handle_mouse_press>(*this);
        dispatcher.sink<events::ScrollWheel>().connect<&PanOrbitSystem::handle_scroll>(*this);
        window_size = shell.window_size();

        auto ui_camera = shell.registry.create();
        shell.registry.emplace<gl::Renderer::ui_func_t>(ui_camera, [](Shell &shell) {
          auto &ctrl = shell.resources().get<components::PanOrbitCamera>();
          ImGui::BeginGroup();
          ImGui::Text("Camera");
          ImGui::InputFloat3("Position", glm::value_ptr(ctrl.eye));
          ImGui::InputFloat3("Target", glm::value_ptr(ctrl.target));
          ImGui::Text("Sensitivities");
          ImGui::InputFloat("Zoom", &ctrl.zoom_sensitivity);
          ImGui::InputFloat("Pan", &ctrl.pan_sensitivity);
          ImGui::InputFloat("Translate", &ctrl.translate_sensitivity);
          ImGui::EndGroup();
        });
    }
    void PanOrbitSystem::execute(Shell &shell) {
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
    void PanOrbitSystem::handle_mouse_press(events::MouseButton button) {
        switch (button.button) {
            case SDL_BUTTON_LEFT:
                left_pressed = button.pressed;
                break;
            case SDL_BUTTON_RIGHT:
                right_pressed = button.pressed;
                break;
            default:
                break;
        }
    }
    void PanOrbitSystem::handle_mouse_move(events::MouseMove move) { relative += move.relative; }
    void PanOrbitSystem::handle_scroll(events::ScrollWheel wheel) { zoom_ticks += wheel.delta.y; }
    void PanOrbitSystem::handle_keys(events::KeyboardEvent event) {
        glm::vec2 right(10, 0), up(0, 10);
        if (!event.pressed) return;
        if (event.is(SDLK_LEFT)) relative -= right;
        if (event.is(SDLK_RIGHT)) relative += right;
        if (event.is(SDLK_UP)) relative += up;
        if (event.is(SDLK_DOWN)) relative -= up;
        if (event.is(SDLK_PAGEUP)) zoom_ticks += 1;
        if (event.is(SDLK_PAGEDOWN)) zoom_ticks -= 1;
    }
}