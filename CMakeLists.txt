cmake_minimum_required(VERSION 3.24)
project(ig_projet CXX C OBJC)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.24.2
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
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.89
)
FetchContent_Declare(
        nfd
        GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
        GIT_TAG v1.0.0
        FIND_PACKAGE_ARGS
        NAMES nfd NFD nativefiledialog nativefiledialog-extended
)
set(NFD_PORTAL ON)
FetchContent_MakeAvailable(openmesh glm EnTT SDL2 spdlog imgui nfd)

set(IMGUI_DIR ${CMAKE_BINARY_DIR}/_deps/imgui-src)
add_library(imgui SHARED ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp ${IMGUI_DIR}/backends/imgui_impl_sdl.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(imgui PRIVATE SDL2::SDL2 glad)

add_subdirectory(lib)
add_subdirectory(src)