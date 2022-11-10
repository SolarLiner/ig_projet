//
// Created by solarliner on 20/10/22.
//

#include "Mesh.h"

#include <utility>
#include <polyscope/polyscope.h>
#include "IOException.h"
#include "OpenMesh/Core/IO/MeshIO.hh"

namespace base {
    Mesh Mesh::open(std::string filename) {
        Mesh out;
        if(!OpenMesh::IO::read_mesh(out, filename))
            throw IOException("read", filename);
        return out;
    }

    void Mesh::save(const std::string& filename) const {
        if(!OpenMesh::IO::write_mesh(*this, filename))
            throw IOException("write", filename);
    }

    polyscope::SurfaceMesh *Mesh::show(std::string name) const {
        return polyscope::registerSurfaceMesh(std::move(name), ps_vertices(), ps_faces());
    }

    std::vector<glm::vec3> Mesh::ps_vertices() const {
        std::vector<glm::vec3> positions;
        positions.reserve(n_vertices());

        for(auto v: vertices()) {
            auto pos = point(v);
            positions.emplace_back(pos[0], pos[1], pos[2]);
        }

        return positions;
    }

    std::vector<F> Mesh::ps_faces() const {
        std::vector<F> v_faces;

        for (auto f: faces()) {
            F face;
            size_t ix = 0;
            for (auto fv_it = cfv_iter(f); fv_it.is_valid(); ++fv_it) {
                face[ix++] = fv_it->idx();
            }
            v_faces.push_back(face);
        }

        return v_faces;
    }

}
