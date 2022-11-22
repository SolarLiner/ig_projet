//
// Created by solarliner on 20/11/22.
//

#ifndef IG_PROJET_PICKINGSYSTEM_H
#define IG_PROJET_PICKINGSYSTEM_H

#include "../events.h"
#include "../systems/System.h"
#include "glow/shaders/Program.h"
#include "glow/shaders/Shader.h"

#include <glow/textures/Framebuffer.h>
#include <glow/textures/Texture.h>

namespace shell::events {
    struct Pick {
        glm::vec2 at;
        entt::entity entity;
    };
}// namespace shell::events
namespace shell::gl {
    using namespace glow::shaders;
    using namespace glow::textures;
    using systems::System;
    class PickingSystem : public System {
    public:
        explicit PickingSystem(const std::filesystem::path &resources_dir);
        PickingSystem(Shader vs, Shader fs);

        void before_run(Shell &shell) override;
        void execute(Shell &shell) override;

    private:
        void handle_resize(events::Resize);
        void handle_click(events::MouseButton);

        Program program;
        std::optional<glm::vec2> do_pick = std::nullopt;
        Texture<std::array<uint32_t, 2>, D2> pick_texture;
        Texture<depth<float>, D2> pick_depth;
        Framebuffer framebuffer;
    };
}// namespace shell::gl

#endif//IG_PROJET_PICKINGSYSTEM_H
