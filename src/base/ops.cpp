//
// Created by solarliner on 20/10/22.
//
#include "ops.h"

std::vector<OpenMesh::SmartVertexHandle> ring(const OpenMesh::SmartVertexHandle &v) {
    std::vector<OpenMesh::SmartVertexHandle> vertices;

    auto handle = v.out();
    while (handle.to() != v.out().to()) {
        vertices.push_back(handle.to());
        handle = handle.to().out();
    }

    return vertices;
}