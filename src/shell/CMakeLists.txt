add_library(shell STATIC Shell.cpp gl/Renderer.cpp gl/Renderer.h gl/Color.cpp gl/resource/shaders/Shader.cpp gl/resource/shaders/Program.cpp)
target_link_libraries(shell
        PUBLIC sfml-window sfml-graphics sfml-system EnTT::EnTT glm
        glad base
)
