//
// Created by Osip Chin on 16.10.2020.
//

#include "log_singleton.h"

#include <memory>

namespace log {

    Logger Logger::instance_ = Logger();

    Logger::Logger() : logger_(nullptr)
    {
    }

    Logger& Logger::get() {
        return instance_;
    }

    std::unique_ptr<BaseLogger>& Logger::get_global_logger() {
        return logger_;
    }

    void Logger::set_global_logger(std::unique_ptr<BaseLogger> logger) {
        logger_ = std::move(logger);
    }

}
