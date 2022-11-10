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
