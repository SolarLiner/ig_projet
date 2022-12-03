//
// Created by solarliner on 19/11/22.
//

#include "Mesh.h"

namespace shell::gl {

    Vertex::Vertex(glm::vec3 vertex, glm::vec3 normal, glm::vec2 uv, Color color, float weight)
            : vertex(vertex), normal(normal), uv(uv), color(color), weight(weight) {}

    void Vertex::declare_vertex_attributes() {
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

    Mesh::Mesh() {
        vao.bind_buffer(vbo);
        vao.bind_buffer(ibo);
    }

    void Mesh::update_from_openmesh(base::Mesh &mesh) {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        vertices.reserve(mesh.n_vertices());

        mesh.request_vertex_status();
        mesh.request_vertex_colors();
        mesh.request_face_normals();
        mesh.request_vertex_normals();
        mesh.request_vertex_texcoords2D();

        mesh.update_face_normals();
        mesh.update_vertex_normals();

        for (auto v: mesh.vertices()) {
            auto pt = mesh.point(v);
            auto normal = mesh.normal(v);
            auto color = mesh.color(v);
            auto uv = mesh.texcoord2D(v);
            vertices.push_back(Vertex({pt[0], pt[1], pt[2]}, {normal[0], normal[1], normal[2]}, {uv[0], uv[1]},
                                      Color(color[0], color[1], color[2])));
        }

        mesh.release_vertex_texcoords2D();
        mesh.release_vertex_normals();
        mesh.release_vertex_colors();
        mesh.release_vertex_status();

        mesh.request_face_status();

        for (auto f: mesh.faces()) {
            for (auto fv_it = mesh.cfv_iter(f); fv_it.is_valid(); ++fv_it) { indices.push_back(fv_it->idx()); }
        }

        mesh.release_face_status();

        ibo.upload(indices);
        vbo.upload(vertices);

        // Rebind arrays because re-uploading data makes the VAO lose its bindings
        vao.bind();
        ibo.bind();
        vbo.bind();
        vao.unbind();
        ibo.unbind();
        vbo.unbind();
    }

    void Mesh::draw(bool wireframe) const {
        vao.bind();
        if (ibo.size() == 0 || vbo.size() == 0) return;// Do nothing if no data has been uploaded yet
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
        glDrawElements(GL_TRIANGLES, (GLsizei) ibo.size(), GL_UNSIGNED_INT, nullptr);
        vao.unbind();
    }
}