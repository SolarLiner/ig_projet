add_library(shell STATIC Shell.cpp gl/Renderer.cpp Color.cpp components/PanOrbitCamera.cpp components/Time.cpp gl/Camera.cpp gl/Mesh.cpp gl/Transform.cpp gl/PickingSystem.cpp gl/PickingSystem.h)
target_link_libraries(shell
        PUBLIC
        SDL2::SDL2 EnTT::EnTT glm spdlog::spdlog
        glow base imgui
)
