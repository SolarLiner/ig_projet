add_library(shell STATIC Shell.cpp Renderer.cpp Renderer.h Color.cpp gl/resource/shaders/Shader.cpp gl/resource/shaders/Program.cpp components/PanOrbitCamera.h systems/PanOrbitSystem.h gl/resource/buffers/UniformBuffer.h)
target_link_libraries(shell
        PUBLIC sfml-window sfml-graphics sfml-system EnTT::EnTT glm
        glad base
#        -DENTT_DISABLE_ASSERT
)
