//
// Created by solarliner on 19/11/22.
//

#ifndef IG_PROJET_SDLEXCEPTION_H
#define IG_PROJET_SDLEXCEPTION_H

#include <SDL.h>
#include <string>

namespace shell {
    class SdlException : public std::exception {
    public:
        static void ensure(bool pred) {
            if(!pred) throw SdlException();
        }

        static void ensure(int retval) {
            ensure(retval != -1);
        }

        template<typename T>
        static void ensure(T *ptr) {
            ensure(ptr != nullptr);
        }

        SdlException() : description(SDL_GetError()) {
            SDL_ClearError();
        }
        std::string description;

        [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
            return description.c_str();
        }
    };
}// namespace shell
#endif//IG_PROJET_SDLEXCEPTION_H
