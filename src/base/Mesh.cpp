//
// Created by solarliner on 20/10/22.
//

#include "Mesh.h"

#include <utility>
#include "IOException.h"
#include "OpenMesh/Core/IO/MeshIO.hh"

namespace base {
    Mesh Mesh::open(const std::string &filename) {
        Mesh out;
        if (!OpenMesh::IO::read_mesh(out, filename))
            throw IOException("read", filename);
        return out;
    }

    void Mesh::save(const std::string &filename) const {
        if (!OpenMesh::IO::write_mesh(*this, filename))
            throw IOException("write", filename);
    }

    void Mesh::reset_vertex_colors() {
        request_vertex_colors();
        for (auto v: vertices()) {
            set_color(v, {255, 255, 255});
        }
        release_vertex_colors();
    }
}
