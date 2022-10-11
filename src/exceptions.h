//
// Created by solarliner on 11/10/22.
//

#ifndef IG_PROJET_EXCEPTIONS_H
#define IG_PROJET_EXCEPTIONS_H

#include <exception>
#include <string>

class ActionException : public std::exception {
public:
    explicit ActionException(const std::string &filename) : message(create_message(action, filename)) {
    }

    ActionException(const std::string &filename, const std::exception &exp) : message(create_message(action, filename, exp)) {}

    char *what() {
        return const_cast<char *>(message.c_str());
    }

protected:
    const char *action;

private:
    static auto create_message(const char *action, const std::string &filename) -> std::string {
        std::stringstream s;
        s << "Cannot " << action << " to file '" << filename << "'";
        return s.str();
    }

    static auto create_message(const char *action, const std::string &filename, const std::exception &exp) -> std::string {
        std::stringstream s;
        s << "Cannot " << action << " to file '" << filename << "': " << exp.what();
        return s.str();
    }

    const std::string message;
};

class OpenException : public ActionException {
    using ActionException::ActionException;

protected:
    const char *action = "open";
};

class SaveException : public ActionException {
    using ActionException::ActionException;

protected:
    const char *action = "save";
};

#endif//IG_PROJET_EXCEPTIONS_H
