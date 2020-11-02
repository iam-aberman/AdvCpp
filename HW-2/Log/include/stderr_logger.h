//
// Created by Osip Chin on 16.10.2020.
//

#ifndef STDERR_LOGGER_H
#define STDERR_LOGGER_H

#include "base_logger.h"
#include <iostream>

namespace log {

    class StderrLogger : public BaseLogger {
    public:
        explicit StderrLogger(Level level = Level::INFO) : BaseLogger(std::cerr, level)
        {
        }
    };


}


#endif //STDERR_LOGGER_H
