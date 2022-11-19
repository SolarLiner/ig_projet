cmake_minimum_required(VERSION 3.24)
project(ig_projet)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.24.2https://github.com/libsdl-org/SDL.git
        FIND_PACKAGE_ARGS
        NAMES sdl2 SDL2 SDL
)
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
        EnTT
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG v3.11.0
        FIND_PACKAGE_ARGS
        NAMES ENTT entt EnTT
)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.11.0
        FIND_PACKAGE_ARGS
        NAMES spdlog
)
FetchContent_MakeAvailable(openmesh glm EnTT SDL2 spdlog)

add_subdirectory(lib)
add_subdirectory(src)