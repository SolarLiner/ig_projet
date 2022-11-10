//
// Created by solarliner on 20/10/22.
//

#ifndef IG_PROJET_IOEXCEPTION_H
#define IG_PROJET_IOEXCEPTION_H


#include <string>
#include <exception>

class IOException: public std::exception {

    char *what();

public:
    IOException(const std::string& action, const std::string& filename);

private:
    static std::string create_message(const std::string& action, const std::string &filename);
    std::string message;
};


#endif//IG_PROJET_IOEXCEPTION_H
