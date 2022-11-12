//
// Created by solarliner on 11/11/22.
//

#include "Shell.h"

namespace renderer {
    void Shell::run() {
        auto &dispatcher = registry.ctx().get<entt::dispatcher>();

        for(auto &system: systems) {
            system->before_run(registry);
        }

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                switch(event.type) {
                    case sf::Event::Closed:
                        dispatcher.enqueue<events::Close>();
                        break;
                    case sf::Event::KeyPressed:
                        dispatcher.enqueue<events::KeyboardEvent>(event.key.code, true);
                        break;
                    case sf::Event::KeyReleased:
                        dispatcher.enqueue<events::KeyboardEvent>(event.key.code, false);
                        break;
                    default: break;
                }
            }
            
            for(auto &system: systems) (*system)(registry);
            dispatcher.update();
        }
        exit(0);
    }

    void Shell::on_close_requested(events::Close) {
        window.close();
    }
}
