#add_library(shell STATIC Shell.cpp gl/Renderer.cpp gl/ Color.cpp ../glow/Shader.cpp ../glow/Program.cpp components/ systems/ ../glow/include/glow/buffers/ components/  components/ sdl/ sdl/ sdl/ sdl/)
add_library(shell STATIC Shell.cpp gl/Renderer.cpp Color.cpp)
target_link_libraries(shell
        PUBLIC SDL2::SDL2 EnTT::EnTT glm
        glow glad base
        PRIVATE
        spdlog::spdlog
)
