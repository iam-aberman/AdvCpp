//
// Created by Osip Chin on 05.11.2020.
//

#ifndef TCP_EXCEPTION_H
#define TCP_EXCEPTION_H

#include <stdexcept>

namespace tcp {
    class descriptor_error : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class connection_error : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };
}

#endif //TCP_EXCEPTION_H
