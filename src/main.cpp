#include "OpenMesh/Core/Mesh/Attributes.hh"
#include "mesh.h"
#include "polyscope_openmesh.h"
#include <cstddef>
#include <iostream>
#include <vector>

template<typename Mesh>
auto barycenters_inplace(Mesh &mesh) -> void {
    std::vector<MyMesh::Point> cogs;
    cogs.reserve(mesh.n_vertices());
    for (auto &&v_it: mesh.vertices()) {
        MyMesh::Scalar valence = 0.0;
        MyMesh::Point cog(0);
        for (auto vv_it: mesh.vv_range(v_it)) {
            cog += mesh.point(vv_it);
            ++valence;
        }

        auto barycenter = cog / valence;
        std::cout << "Barycenter: (" << barycenter << ")" << std::endl;
        cogs.push_back(barycenter);
    }

    size_t i = 0;
    for (auto &&vert: mesh.vertices()) {
        mesh.set_point(vert, cogs[i++]);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Please provide mesh filename" << std::endl;
        return 1;
    }

    polyscope::init();

    auto mesh = PolyscopeMesh::read_mesh(argv[1]);
//    barycenters_inplace(mesh);
    mesh.save_mesh("barycenters.obj");

    polyscope::registerSurfaceMesh("mesh", mesh.ps_vertices(), mesh.ps_faces());

    polyscope::show();

    return 0;
}
