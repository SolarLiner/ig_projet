cmake_minimum_required(VERSION 3.23)
project(ig_projet)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# openmesh
FetchContent_Declare(
    openmesh
    GIT_REPOSITORY https://gitlab.vci.rwth-aachen.de:9000/OpenMesh/OpenMesh.git
    GIT_TAG OpenMesh-9.0
)

FetchContent_Declare(
    polyscope
    GIT_REPOSITORY https://github.com/nmwsharp/polyscope.git
    GIT_TAG v1.3.0
)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(openmesh polyscope glm)

add_subdirectory(src)
