//
// Created by Osip Chin on 16.10.2020.
//

#include "log.h"
#include "stdout_logger.h"
#include "stderr_logger.h"
#include "file_logger.h"

namespace log {

    void init_with_stdout_logger(Level level) {
        Logger::get().set_global_logger(std::make_unique<StdoutLogger>(level));
    }

    void init_with_stderr_logger(Level level) {
        Logger::get().set_global_logger(std::make_unique<StderrLogger>(level));
    }

    void init_with_file_logger(const std::string& filename, Level level) {
        Logger::get().set_global_logger(std::make_unique<FileLogger>(filename, level));
    }

    void set_level(Level level) {
        if (!Logger::get().is_valid()) {
            init_with_stderr_logger();
        }
        Logger::get().get_global_logger()->set_level(level);
    }

    void debug(const std::string& msg) {
        if (!Logger::get().is_valid()) {
            init_with_stderr_logger();
        }
        Logger::get().get_global_logger()->debug(msg);
    }

    void info(const std::string& msg) {
        if (!Logger::get().is_valid()) {
            init_with_stderr_logger();
        }
        Logger::get().get_global_logger()->info(msg);
    }

    void warn(const std::string& msg) {
        if (!Logger::get().is_valid()) {
            init_with_stderr_logger();
        }
        Logger::get().get_global_logger()->warn(msg);
    }

    void error(const std::string& msg) {
        if (!Logger::get().is_valid()) {
            init_with_stderr_logger();
        }
        Logger::get().get_global_logger()->error(msg);
    }

}
