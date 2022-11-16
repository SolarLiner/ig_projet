//
// Created by solarliner on 11/11/22.
//

#include "Shell.h"
#include <chrono>
#include <cstdarg>
#include <iostream>
#include "components/Time.h"

/*
void debug_callback(const char* mesg, void*, int nargs, ...) {
    va_list args;
    va_start(args, nargs);
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << std::ctime(&now) << "\t" << mesg << "(";
    for(int i = 0; i < nargs; ++i) {
        if(i>0) std::cout << ", ";
        std::cout << va_arg(args, int);
    }
    std::cout << ")" << std::endl;
}
*/

static sf::ContextSettings default_context() {
    sf::ContextSettings settings;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;
    settings.depthBits = 24;
    return settings;
}

namespace shell {
    Shell::Shell(size_t w, size_t h, const std::string &win_name)
        : window(sf::VideoMode(w, h), win_name, sf::Style::Default, default_context()) {
        auto &dispatcher = registry.ctx().emplace<entt::dispatcher>();
        dispatcher.sink<events::Close>().connect<&Shell::on_close_requested>(*this);
        gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction));
    }

    void Shell::run() {
        auto &dispatcher = registry.ctx().get<entt::dispatcher>();
        window.setVerticalSyncEnabled(true);

        for (auto &system: systems) { system->before_run(window, registry); }

        auto &time = registry.ctx().emplace<components::Time>();
        while (window.isOpen()) {
            poll_events(dispatcher);
            for (auto &system: systems) (*system)(window, registry);
            dispatcher.update();
            time.last_frame = time.frame.getElapsedTime();
            time.frame.restart();
        }
        exit(0);
    }
    void Shell::poll_events(entt::dispatcher &dispatcher) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    dispatcher.enqueue<events::Close>();
                    break;
                case sf::Event::Resized:
                    dispatcher.enqueue<events::Resize>(event.size.width, event.size.height);
                    break;
                case sf::Event::KeyPressed:
                    dispatcher.enqueue<events::KeyboardEvent>(event.key.code, true);
                    break;
                case sf::Event::KeyReleased:
                    dispatcher.enqueue<events::KeyboardEvent>(event.key.code, false);
                    break;
                case sf::Event::MouseMoved:
                    dispatcher.enqueue<events::MouseMove>(glm::vec2(event.mouseMove.x, event.mouseMove.y));
                    break;
                case sf::Event::MouseButtonPressed:
                    dispatcher.enqueue<events::MouseButton>(true, event.mouseButton.button,
                                                            glm::vec2(event.mouseButton.x, event.mouseButton.y));
                    break;
                case sf::Event::MouseButtonReleased:
                    dispatcher.enqueue<events::MouseButton>(false, event.mouseButton.button,
                                                            glm::vec2(event.mouseButton.x, event.mouseButton.y));
                case sf::Event::MouseWheelScrolled:
                    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                        dispatcher.enqueue<events::ScrollWheel>(event.mouseWheelScroll.delta);
                    break;
                default:
                    break;
            }
        }
    }

    void Shell::on_close_requested(events::Close) { window.close(); }
}// namespace shell
