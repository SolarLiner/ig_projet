#include "SFML/Window/Keyboard.hpp"
#if 0
#include "base/Mesh.h"
#include "transforms/Laplace.h"

int main(int argc, char **argv) {
    transforms::Laplace transform(0.5);
    polyscope::init();

//    auto mesh = cube<PolyscopeMesh>();
    auto mesh = Mesh::open("suzanne.obj");
    auto p_input = mesh.show("Suzanne");

    transform.smoothen(mesh);
    auto p_smooth = mesh.show("laplace iter 1");

    int i = 1;
    for (; i < 10; ++i) transform.smoothen(mesh);
    auto p_smoother = mesh.show("laplace iter 10");

    for (; i < 100; ++i) transform.smoothen(mesh);
    auto p_smoother_still = mesh.show("laplace iter 100");

    polyscope::show();

    return 0;
}
#else
#include "renderer/Shell.h"
#include <iostream>
#include <memory>

using renderer::Shell;
using Key = sf::Keyboard::Key;

class EscapeExit: public renderer::System {
    public:
        void before_run(entt::registry &registry) override {
            dispatcher = &registry.ctx().get<entt::dispatcher>();
            dispatcher->sink<renderer::events::KeyboardEvent>().connect<&EscapeExit::on_key_pressed>(*this);
        }

    private:
        void on_key_pressed(renderer::events::KeyboardEvent event) {
            if(event.key == Key::Escape) dispatcher->enqueue<renderer::events::Close>();
        }

        entt::dispatcher *dispatcher;
};

int main(int argc, char **argv) {
    Shell shell;
    shell.add_system(new EscapeExit());
    shell.run();
}

#endif
