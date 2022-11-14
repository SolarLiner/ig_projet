//
// Created by solarliner on 11/11/22.
//

#ifndef IG_PROJET_SHELL_H
#define IG_PROJET_SHELL_H

#include "Color.h"
#include "System.h"
#include "events.h"
#include "gl/Camera.h"
#include "gl/ClearColor.h"
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <glad.h>
#include <memory>
#include <optional>
#include <utility>

namespace shell {

    class Shell {
    public:
        Shell() : Shell(800, 600) {}

        Shell(size_t w, size_t h, const std::string &win_name = "Not Polyscope");

        void add_system(FunctionSystem::system_t system) {
            add_system(new FunctionSystem(std::move(system)));
        }


        void add_system(System *system) {
            systems.emplace_back(system);
        }

        [[noreturn]] void run();

        decltype(auto) setup_default_environment() {
            auto &context = registry.ctx();
            context.emplace<gl::Camera>();
            registry.ctx().insert_or_assign(gl::ClearColor{Color(0.1f, 0.1f, 0.1f)});
            return context;
        }

        entt::registry registry;

    private:
        void on_close_requested(events::Close event);

        sf::Window window;
        std::list<std::unique_ptr<System>> systems;
        void poll_events(entt::dispatcher &dispatcher);
    };
}// namespace shell


#endif//IG_PROJET_SHELL_H
