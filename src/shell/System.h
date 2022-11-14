//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_SYSTEM_H
#define IG_PROJET_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <functional>
#include <memory>
#include <optional>
#include <utility>

namespace shell {
    class System {
    public:
        virtual void before_run(const sf::Window &window, entt::registry &registry) {}
        virtual void operator()(const sf::Window &window, entt::registry &registry) {}
    };

    class FunctionSystem : public System {
    public:
        typedef std::function<void(const sf::Window &, entt::registry &)> system_t;
        explicit FunctionSystem(system_t system) : system(std::move(system)) {}

        void operator()(const sf::Window &window, entt::registry &registry) override {
            system(window, registry);
        }

    private:
        system_t system;
    };
}
#endif//IG_PROJET_SYSTEM_H
