#include "polyscope_openmesh.h"
#include "exceptions.h"

std::vector<PolyscopeMesh::Point> PolyscopeMesh::ps_vertices() const {
    std::vector<Point> positions(n_vertices());

    for(auto v: vertices()) {
        auto pos = point(v);
        positions.push_back(pos);
    }

    return positions;
}

std::vector<std::array<unsigned int, 3>> PolyscopeMesh::ps_faces() const {
    std::vector<std::array<unsigned int, 3>> faces(n_faces());

    for (auto f: this->faces()) {
        std::array<unsigned int, 3> face;
        size_t ix = 0;
        for (auto fv_it = cfv_iter(f); fv_it.is_valid(); ++fv_it) {
            face[ix++] = fv_it->idx();
        }
        faces.push_back(face);
    }

    return faces;
}

PolyscopeMesh PolyscopeMesh::read_mesh(const std::string &filename) {
    PolyscopeMesh mesh;
    if(!OpenMesh::IO::read_mesh(mesh, filename)) {
        throw OpenException(filename);
    }
    return mesh;
}

void PolyscopeMesh::save_mesh(const std::string &filename) const {
    if(!OpenMesh::IO::write_mesh(*this, filename)) throw SaveException(filename);
}
