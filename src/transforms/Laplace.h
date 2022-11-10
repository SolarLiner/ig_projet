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
        explicit Laplace(V::value_type alpha);
        Laplace(): alpha(0.5) {}

        Mesh smoothen(const Mesh &input) const;

        void smoothen(Mesh &mesh) const;

    private:
        V::value_type alpha;
    };
}


#endif//IG_PROJET_LAPLACE_H
