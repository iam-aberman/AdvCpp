//
// Created by Osip Chin on 16.10.2020.
//

#ifndef STDOUT_LOGGER_H
#define STDOUT_LOGGER_H

#include "base_logger.h"

namespace log {

    class StdoutLogger : public BaseLogger {
    public:
        explicit StdoutLogger(Level level = Level::INFO) : BaseLogger(std::cout, level)
        {
        }
    };

}


#endif //STDOUT_LOGGER_H
