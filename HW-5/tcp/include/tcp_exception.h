//
// Created by Osip Chin on 05.11.2020.
//

#ifndef TCP_EXCEPTION_H
#define TCP_EXCEPTION_H

#include <stdexcept>

namespace tcp {

    class Error : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class DescriptorError : public Error {
    public:
        using Error::Error;
    };

    class ConnectionError : public Error {
    public:
        using Error::Error;
    };
}

#endif //TCP_EXCEPTION_H
