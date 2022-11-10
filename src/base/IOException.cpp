//
// Created by solarliner on 20/10/22.
//

#include "IOException.h"

#include <sstream>
#include <utility>

IOException::IOException(const std::string& action, const std::string& filename): message(create_message(action, filename)) {
}

char *IOException::what() {
    return const_cast<char *>(message.c_str());
}

std::string IOException::create_message(const std::string& action, const std::string &filename) {
    std::stringstream s;
    s << "Cannot " << action << " to file '" << filename << "'";
    return s.str();
}
