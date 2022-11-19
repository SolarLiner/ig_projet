//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_RENDERER_H
#define IG_PROJET_RENDERER_H

#include "../Color.h"
#include "../Shell.h"
#include "../components/Light.h"
#include "../events.h"
#include "../systems/System.h"
#include "ClearColor.h"
#include <cstddef>
#include <glad.h>
#include <glm/vec2.hpp>
#include <glow/buffers/UniformBuffer.h>

namespace shell::gl {
    using glow::buffers::UniformBuffer;

    class Renderer : public shell::systems::System {
    public:
        struct wireframe {};
        Renderer() = default;
        void before_run(Shell &) override;
        void execute(Shell &) override;
        void handle_resize(events::Resize resize);

    private:
        struct Lights {
            static constexpr size_t MAX_LIGHTS = 16;// To be synced with shader.frag
            int num_lights{0};
            int __pad0[3]{};
            components::Light lights[MAX_LIGHTS]{};
        };
        UniformBuffer<Renderer::Lights> lights{1};
        std::optional<glm::vec2> size_changed{};
    };

}// namespace shell::gl

#endif//IG_PROJET_RENDERER_H