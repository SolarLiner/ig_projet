//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_RENDERER_H
#define IG_PROJET_RENDERER_H

#include "Color.h"
#include "SFML/Graphics.hpp"
#include "events.h"
#include "gl/ClearColor.h"
#include "glad.h"
#include "glm/vec2.hpp"
#include "systems/System.h"

namespace shell::gl {
    class Renderer : public shell::systems::System {
    public:
        struct wireframe {};
        void before_run(const sf::Window &window, entt::registry &) override;
        void operator()(const sf::Window &, entt::registry &) override;
        void handle_resize(events::Resize resize);
    private:
        std::optional<glm::vec2> size_changed{};
    };

}// namespace shell::gl

#endif//IG_PROJET_RENDERER_H
