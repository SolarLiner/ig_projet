//
// Created by solarliner on 20/10/22.
//

#ifndef IG_PROJET_LAPLACE_H
#define IG_PROJET_LAPLACE_H

#include "../base/Mesh.h"

using namespace base;

namespace transforms {
    class Laplace {
    public:
        explicit Laplace(float alpha);
        Laplace(): alpha(0.5) {}

        void operator()(Mesh &mesh) const;

    private:
        float alpha;
    };
}


#endif//IG_PROJET_LAPLACE_H
