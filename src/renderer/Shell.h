//
// Created by solarliner on 11/11/22.
//

#ifndef IG_PROJET_SHELL_H
#define IG_PROJET_SHELL_H

#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Window.hpp"
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <memory>
#include <optional>

namespace renderer {
    namespace events {
        struct KeyboardEvent {
            sf::Keyboard::Key key;
            bool pressed;
        };

        struct Close {};
    }// namespace events

    class Shell;
    class System {
    public:
        virtual void before_run(entt::registry &registry) {}
        virtual void operator()(entt::registry &registry) {}
    };

    class FunctionSystem : public System {
    public:
        typedef std::function<void(entt::registry &)> system_t;
        FunctionSystem(system_t system) : system(system) {}

        void operator()(entt::registry &registry) override {
            system(registry);
        }

    private:
        system_t system;
    };

    class Shell {
    public:
        typedef sf::Window window_t;
        typedef std::function<void(entt::registry &)> system_t;
        typedef entt::sigh<void(sf::Keyboard::Key, bool)> key_signal_t;

        Shell() : Shell(800, 600) {}

        Shell(size_t w, size_t h, const std::string &win_name = "Not Polyscope") : window(sf::VideoMode(w, h), win_name) {
            auto &dispatcher = registry.ctx().emplace<entt::dispatcher>();
            dispatcher.sink<events::Close>().connect<&Shell::on_close_requested>(*this);
        }

        void add_system(const std::function<void()> system) {
            add_system([=](auto &reg) { system(); });
        }
        void add_system(FunctionSystem::system_t system) {
            add_system(FunctionSystem(system));
        }


        void add_system(System *system) {
            systems.emplace_back(system);
        }

        [[noreturn]] void run();


    private:
        void on_close_requested(events::Close event);

        window_t window;
        entt::registry registry;
        std::list<std::unique_ptr<System>> systems;
    };
}// namespace renderer


#endif//IG_PROJET_SHELL_H
