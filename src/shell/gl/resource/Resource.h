//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_RESOURCE_H
#define IG_PROJET_RESOURCE_H

#include <glad.h>
#include <string>

class Resource {
public:
    Resource(const Resource &) = delete;
    Resource &operator=(const Resource &) = delete;

    virtual explicit operator GLuint() const = 0;
protected:
    Resource() = default;
};

#endif//IG_PROJET_RESOURCE_H
