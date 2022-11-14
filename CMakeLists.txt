cmake_minimum_required(VERSION 3.24)
project(ig_projet)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# openmesh
FetchContent_Declare(
        openmesh
        GIT_REPOSITORY https://gitlab.vci.rwth-aachen.de:9000/OpenMesh/OpenMesh.git
        GIT_TAG OpenMesh-9.0
        FIND_PACKAGE_ARGS
        NAMES openmesh OpenMesh
)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8
        FIND_PACKAGE_ARGS
        NAMES glm GLM
)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.5.1
        FIND_PACKAGE_ARGS
        NAMES SFML
        COMPONENTS system window graphics
)
FetchContent_Declare(
        EnTT
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG v3.11.0
        FIND_PACKAGE_ARGS
        NAMES ENTT entt EnTT
)
FetchContent_MakeAvailable(openmesh glm SFML EnTT)

add_subdirectory(lib)
add_subdirectory(src)
