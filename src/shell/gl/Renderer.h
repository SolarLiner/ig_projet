//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_RENDERER_H
#define IG_PROJET_RENDERER_H

#include "../Color.h"
#include "../components/Light.h"
#include "../events.h"
#include "../systems/System.h"
#include "ClearColor.h"
#include "SFML/Graphics.hpp"
#include <glad.h>
#include <glm/vec2.hpp>
#include <glow/buffers/UniformBuffer.h>
#include <cstddef>

namespace shell::gl {
    using glow::buffers::UniformBuffer;

    class Renderer : public shell::systems::System {
    public:
        Renderer();
        struct wireframe {};
        void before_run(const sf::Window &window, entt::registry &) override;
        void operator()(const sf::Window &, entt::registry &) override;
        void handle_resize(events::Resize resize);
    private:
        struct Lights {
            static constexpr size_t MAX_LIGHTS = 16; // To be synced with shader.frag
            int num_lights{0};
            int __pad0[3]{};
            components::Light lights[MAX_LIGHTS]{};
        };
        UniformBuffer<Renderer::Lights> lights{1};
        std::optional<glm::vec2> size_changed{};
    };

}// namespace shell::gl

#endif//IG_PROJET_RENDERER_H