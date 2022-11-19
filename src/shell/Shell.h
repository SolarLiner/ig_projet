//
// Created by solarliner on 11/11/22.
//

#ifndef IG_PROJET_SHELL_H
#define IG_PROJET_SHELL_H

#include "Color.h"
#include "SdlException.h"
#include "events.h"
#include "gl/Camera.h"
#include "gl/ClearColor.h"
#include "systems/System.h"
#include <SDL.h>
#include <entt/entt.hpp>
#include <glad.h>
#include <memory>
#include <optional>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#include <utility>

namespace shell {
    class Shell {
    public:
        void add_system(systems::FunctionSystem::system_t system);

        template<typename T, typename... Args>
        void emplace_system(Args &&...args) {
            //            systems.template emplace(std::make_unique<T>(args...));
            systems.emplace_back(std::make_unique<T>(args...));
        }

        Shell();
        ~Shell();
        void init();
        void add_system(systems::System *system) { systems.emplace_back(system); }
        [[noreturn]] void run();

        template<typename T>
        entt::sink<T> &event() {
            return registry.ctx().get<entt::dispatcher>().sink<T>();
        }

        entt::registry::context &resources();
        void setup_default_environment();

        glm::vec2 window_size() const;
        glm::vec2 viewport_size() const;

        void swap_buffers() const;

        entt::registry registry;
        bool should_close = false;

    private:
        void on_close_requested(events::Close event);
        void ensure_init() const;

        SDL_Window *window;
        SDL_GLContext context;
        std::list<std::unique_ptr<systems::System>> systems;
        void poll_events(entt::dispatcher &dispatcher);
    };

}// namespace shell

#endif//IG_PROJET_SHELL_H
