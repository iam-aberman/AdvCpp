#include "logger.h"

namespace log {

    const std::map<Level, std::string> BaseLogger::level_to_prefix_ = {
            {Level::Debug, "[Debug] "},
            {Level::Info, "[Info] "},
            {Level::Warning, "[Warning] "},
            {Level::Error, "[Error] "}
    };

    BaseLogger::BaseLogger(Level level) : level_(level)
    {
    }

    void BaseLogger::debug(const std::string& msg) {
        log(msg, Level::Debug);
    }

    void BaseLogger::info(const std::string& msg) {
        log(msg, Level::Info);
    }

    void BaseLogger::warn(const std::string& msg) {
        log(msg, Level::Warning);
    }

    void BaseLogger::error(const std::string& msg) {
        log(msg, Level::Error);
    }

    void BaseLogger::set_level(Level level) {
        level_ = level;
    }

    Level BaseLogger::get_level() const {
        return level_;
    }

}
