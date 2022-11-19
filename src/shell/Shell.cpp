//
// Created by solarliner on 11/11/22.
//

#include "Shell.h"
#include "components/Time.h"
#include <SDL.h>
#include <cstdarg>
#include <entt/signal/fwd.hpp>
#include <glad.h>
#include <iostream>
#include <spdlog/fmt/ranges.h>
#include <spdlog/spdlog.h>

static void print_opengl_calls(const char *name, void *, int nargs, ...) {
    va_list args;
    std::vector<int> data(nargs);
    va_start(args, nargs);
    for (int i = 0; i < nargs; ++i) { data[i] = va_arg(args, int); }

    spdlog::trace("[OpenGL] {} {}", name, data);
}

void APIENTRY gl_debug_msg_cb(GLenum source, GLenum type, GLuint, GLenum severity, GLsizei, const GLchar *msg,
                              const void *) {
    const char *_source;
    const char *_type;
    auto level = spdlog::level::debug;

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            _source = "API";
            break;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            _source = "WINDOW SYSTEM";
            break;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            _source = "SHADER COMPILER";
            break;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
            _source = "THIRD PARTY";
            break;

        case GL_DEBUG_SOURCE_APPLICATION:
            _source = "APPLICATION";
            break;

        case GL_DEBUG_SOURCE_OTHER:
            _source = "UNKNOWN";
            break;

        default:
            _source = "UNKNOWN";
            break;
    }

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            _type = "ERROR";
            break;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            _type = "DEPRECATED BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            _type = "UDEFINED BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_PORTABILITY:
            _type = "PORTABILITY";
            break;

        case GL_DEBUG_TYPE_PERFORMANCE:
            _type = "PERFORMANCE";
            break;

        case GL_DEBUG_TYPE_OTHER:
            _type = "OTHER";
            break;

        case GL_DEBUG_TYPE_MARKER:
            _type = "MARKER";
            break;

        default:
            _type = "UNKNOWN";
            break;
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            level = spdlog::level::err;
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            level = spdlog::level::warn;
            break;

        case GL_DEBUG_SEVERITY_LOW:
            level = spdlog::level::info;
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            level = spdlog::level::debug;
            break;

        default:
            break;
    }

    spdlog::log(level, "[OpenGL {:<6} {:>6}] {}", _source, _type, msg);
}

namespace shell {
    Shell::Shell() : window(nullptr), context(nullptr) { registry.ctx().emplace<entt::dispatcher>(); }

    static constexpr void ensure_gl_attribute(SDL_GLattr attrib, int value) {
        SdlException::ensure(SDL_GL_SetAttribute(attrib, value));
    }

    Shell::~Shell() {
        if (context) SDL_GL_DeleteContext(context);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void Shell::init() {
        spdlog::trace("Shell::init");
        if (window && context) return;

        SdlException::ensure(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS));
        spdlog::debug("SDL initialized");

        window = SDL_CreateWindow("Not Polyscope", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
        SdlException::ensure(window);
        spdlog::debug("Created SDL window {:x}", (size_t) window);
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        spdlog::debug("SDL window length {}x{}", w, h);

        ensure_gl_attribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        ensure_gl_attribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        ensure_gl_attribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        ensure_gl_attribute(SDL_GL_RED_SIZE, 8);
        ensure_gl_attribute(SDL_GL_GREEN_SIZE, 8);
        ensure_gl_attribute(SDL_GL_BLUE_SIZE, 8);
        ensure_gl_attribute(SDL_GL_ALPHA_SIZE, 8);
        ensure_gl_attribute(SDL_GL_DEPTH_SIZE, 24);
        ensure_gl_attribute(SDL_GL_DOUBLEBUFFER, 1);
        ensure_gl_attribute(SDL_GL_ACCELERATED_VISUAL, 1);

        SDL_GL_LoadLibrary(nullptr);
        context = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, context);
        gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);
        glad_set_pre_callback(print_opengl_calls);
        spdlog::debug("OpenGL loaded");

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(gl_debug_msg_cb, nullptr);
    }

    void Shell::run() {
        ensure_init();
        spdlog::debug("Shell::run");
        auto &dispatcher = registry.ctx().get<entt::dispatcher>();
        dispatcher.sink<events::Close>().connect<&Shell::on_close_requested>(*this);

        for (auto &system: systems) { system->before_run(*this); }

        auto &time = registry.ctx().emplace<components::Time>();
        time.start = components::Time::now();
        while (!should_close) {
            poll_events(dispatcher);
            auto poll_time = time.since_frame();
            for (auto &system: systems) system->execute(*this);
            auto systems_time = time.since_frame() - poll_time;
            dispatcher.update();
            time.last_frame = time.since_frame();
            time.frame = components::Time::now();
            spdlog::info("frame {} ms\n\tpolling {} ms\n\tsystems {} ms", time.last_frame.count(), poll_time.count(), systems_time.count());
        }
        exit(0);
    }

    void Shell::poll_events(entt::dispatcher &dispatcher) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            spdlog::debug("SDL event {}", event.type);
            switch (event.type) {
                case SDL_QUIT:
                    dispatcher.enqueue<events::Close>();
                    break;
                case SDL_WINDOWEVENT:
                    switch(event.window.event) {
                        case SDL_WINDOWEVENT_CLOSE:
                            should_close = true;
                            break;
                        case SDL_WINDOWEVENT_RESIZED:
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                            dispatcher.enqueue<events::Resize>(glm::vec2 {event.window.data1, event.window.data2});
                            break;
                        default: break;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    dispatcher.enqueue<events::MouseMove>(glm::vec2 {event.motion.x, event.motion.y}, glm::vec2 {event.motion.xrel, event.motion.yrel});
                    break;
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEBUTTONDOWN:
                    dispatcher.enqueue<events::MouseButton>(event.button.state == SDL_PRESSED, event.button.button, glm::vec2 {event.button.x, event.button.y});
                    break;
                case SDL_MOUSEWHEEL:
                    dispatcher.enqueue<events::ScrollWheel>(glm::vec2 {event.wheel.preciseX, event.wheel.preciseY});
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    dispatcher.enqueue<events::KeyboardEvent>(event.key.keysym, event.key.state == SDL_PRESSED);
                    break;
                default:
                    break;
            }
        }
    }

    void Shell::on_close_requested(events::Close) { should_close = true; }

    void Shell::setup_default_environment() {
        ensure_init();
        resources().emplace<gl::Camera>();
        resources().insert_or_assign(gl::ClearColor{Color(0.1f, 0.1f, 0.1f)});
    }

    void Shell::ensure_init() const {
        class InitException : public std::exception {
        public:
            [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
                return "Shell has not been initialized - please run the init() function before anything else";
            }
        };

        if (!window || !context) throw InitException();
    }
    void Shell::add_system(systems::FunctionSystem::system_t system) {
        add_system(new systems::FunctionSystem(std::move(system)));
    }

    entt::registry::context &Shell::resources() { return registry.ctx(); }
    glm::vec2 Shell::window_size() const {
        int w = 0, h = 0;
        SDL_GetWindowSize(window, &w, &h);
        return {w, h};
    }
    glm::vec2 Shell::viewport_size() const {
        int w = 0, h = 0;
        SDL_GL_GetDrawableSize(window, &w, &h);
        return {w, h};
    }
    void Shell::swap_buffers() const { SDL_GL_SwapWindow(window); }
}// namespace shell
