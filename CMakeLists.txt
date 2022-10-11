cmake_minimum_required(VERSION 3.23)
project(ig_projet)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# openmesh
find_package(openmesh QUIET)
if (NOT openmesh_FOUND)
    FetchContent_Declare(
        openmesh
        GIT_REPOSITORY https://gitlab.vci.rwth-aachen.de:9000/OpenMesh/OpenMesh.git
        GIT_TAG OpenMesh-9.0
    )
    FetchContent_MakeAvailable(openmesh)
endif()

FetchContent_Declare(
    polyscope
    GIT_REPOSITORY https://github.com/nmwsharp/polyscope.git
    GIT_TAG v1.3.0
)
FetchContent_MakeAvailable(polyscope)

add_executable(ig_projet src/main.cpp src/polyscope_openmesh.cpp)
target_link_libraries(ig_projet OpenMeshCore polyscope)
