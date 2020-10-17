//
// Created by Osip Chin on 16.10.2020.
//

#include "stdout_logger.h"

#include <iostream>

namespace log {

    StdoutLogger::StdoutLogger(Level level) : BaseLogger(level)
    {
    }

    void StdoutLogger::flush() {
        std::cout.flush();
    }

    void StdoutLogger::log(const std::string& msg, Level level) {
        if (level >= get_level()) {
            std::cout << msg + '\n';
        }
    }

}