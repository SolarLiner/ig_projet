//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_EVENTLISTENER_H
#define IG_PROJET_EVENTLISTENER_H

#include "System.h"
#include <entt/entt.hpp>
#include <memory>
#include <optional>

namespace shell::systems {
    template<typename Event>
    class EventListener : public shell::systems::System {
    public:
        typedef std::function<void(entt::registry &, Event)> handler_t;
        explicit EventListener(handler_t handler) : registry(), handler(handler) {}

        void before_run(Shell &shell) override {
            registry = &shell.registry;
            shell.resources().get<entt::dispatcher>().sink<Event>().template connect<&EventListener::handle_event>(
                    *this);
        }

    private:
        void handle_event(Event event) { handler(*registry, event); }
        entt::registry *registry;
        handler_t handler;
    };
}// namespace shell::systems
#endif//IG_PROJET_EVENTLISTENER_H
