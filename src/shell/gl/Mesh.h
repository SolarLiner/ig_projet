//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_SHELL_MESH_H
#define IG_PROJET_SHELL_MESH_H

#include "../../base/Mesh.h"
#include "../Color.h"
#include <glow/buffers/Buffer.h>
#include <glow/buffers/VertexArray.h>
#include <glow/buffers/VertexBuffer.h>
#include <glow/buffers/IndexBuffer.h>
#include <glm/glm.hpp>

namespace shell::gl {
    using namespace glow::buffers;
    struct Vertex {
        Vertex(glm::vec3 vertex, glm::vec3 normal, glm::vec2 uv = glm::vec2(0.f, 0.f), Color color = Color::Black,
               float weight = 0.f);

        static void declare_vertex_attributes();

        glm::vec3 vertex;
        glm::vec3 normal;
        glm::vec2 uv;
        Color color;
        float weight;
    };

    struct Mesh {
    public:
        VertexBuffer<Vertex, Dynamic> vbo{};
        IndexBuffer<uint32_t, Dynamic> ibo{};
        VertexArray vao{};

        Mesh();

        void update_from_openmesh(base::Mesh &mesh);

        void draw(bool wireframe = false) const;
    };
}// namespace shell::gl

#endif//IG_PROJET_SHELL_MESH_H
