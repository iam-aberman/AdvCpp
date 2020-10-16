//
// Created by Osip Chin on 16.10.2020.
//

#include "stdout_logger.h"

#include <iostream>

namespace log {

    StdoutLogger::StdoutLogger() : BaseLogger()
    {
    }

    void StdoutLogger::flush() {
        std::cout.flush();
    }

    void StdoutLogger::log(const std::string& msg, Level level) {
        if (level >= get_level()) {
            std::cout << level_to_prefix_.at(level) << msg + '\n';
        }
    }

}