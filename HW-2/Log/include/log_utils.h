//
// Created by Osip Chin on 16.10.2020.
//

#ifndef LOG_UTILS_H
#define LOG_UTILS_H

#include "log_singleton.h"

namespace log {

    void init_with_stdout_logger(Level level = Level::Debug);
    void init_with_stderr_logger(Level level = Level::Debug);
    void init_with_file_logger(const std::string& filename, Level level = Level::Debug);

    void set_level(Level level);

    void debug(const std::string& msg);
    void info(const std::string& msg);
    void warn(const std::string& msg);
    void error(const std::string& msg);
}

#endif //LOG_UTILS_H
