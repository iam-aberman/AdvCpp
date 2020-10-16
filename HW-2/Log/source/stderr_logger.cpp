//
// Created by Osip Chin on 16.10.2020.
//

#include "stderr_logger.h"

#include <iostream>

namespace log {

    StderrLogger::StderrLogger(Level level) : BaseLogger(level)
    {
    }

    void StderrLogger::flush()
    {
    }

    void StderrLogger::log(const std::string& msg, Level level) {
        if (level >= get_level()) {
            std::cerr << level_to_prefix_.at(level) << msg + '\n';
        }
    }

}
