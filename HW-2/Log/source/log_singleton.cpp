//
// Created by Osip Chin on 16.10.2020.
//

#include "log_singleton.h"
#include "stderr_logger.h"

#include <memory>

namespace log {

    Logger Logger::instance_ = Logger();

    Logger::Logger() : logger_(std::make_unique<StderrLogger>())
    {
    }

    Logger& Logger::get() {
        return instance_;
    }

    std::unique_ptr<BaseLogger>& Logger::get_global_logger() {
        return logger_;
    }

    void Logger::set_global_logger(std::unique_ptr<BaseLogger> logger) {
        logger_.swap(logger);
    }

    bool Logger::is_valid() const {
        return logger_ != nullptr;
    }

}
