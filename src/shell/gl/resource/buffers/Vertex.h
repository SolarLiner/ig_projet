//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_VERTEX_H
#define IG_PROJET_VERTEX_H

#include "../../../Color.h"
#include "glm/glm.hpp"

namespace shell::gl::resource {
    struct Vertex {
        Vertex(glm::vec3 vertex, glm::vec3 normal, glm::vec2 uv = glm::vec2(0.f, 0.f), Color color = Color::Black, float weight = 0.f) : vertex(vertex), normal(normal), uv(uv), color(color), weight(weight) {}
        static void setup() {
            // Vertex
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) nullptr);
            glEnableVertexAttribArray(0);

            // Normal
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));
            glEnableVertexAttribArray(1);

            // UV/Texcoords
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, uv));
            glEnableVertexAttribArray(2);

            // Vertex color
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, color));
            glEnableVertexAttribArray(3);

            // Vertex weight
            glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, weight));
            glEnableVertexAttribArray(4);
        }

        glm::vec3 vertex;
        glm::vec3 normal;
        glm::vec2 uv;
        Color color;
        float weight;
    };
}// namespace shell::gl::resource


#endif//IG_PROJET_VERTEX_H
