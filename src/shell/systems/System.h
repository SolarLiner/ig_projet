//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_SYSTEM_H
#define IG_PROJET_SYSTEM_H

#include <entt/entt.hpp>
#include <functional>
#include <memory>
#include <optional>
#include <utility>

namespace shell {
    class Shell;
}
namespace shell::systems {
    class System {
    public:
        virtual void before_run(Shell &shell) {}
        virtual void execute(Shell &shell) {}
    };

    class FunctionSystem : public System {
    public:
        typedef std::function<void(Shell &)> system_t;
        explicit FunctionSystem(system_t system) : system(std::move(system)) {}

        void execute(Shell &shell) override { system(shell); }

    private:
        system_t system;
    };
}// namespace shell::systems
#endif//IG_PROJET_SYSTEM_H
