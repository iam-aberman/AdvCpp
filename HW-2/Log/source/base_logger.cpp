#include "base_logger.h"

namespace log {

    BaseLogger::BaseLogger(std::ostream& os, Level level) : level_(level),
                                                            output_(os)
    {
    }

    void BaseLogger::debug(const std::string& msg) {
        log("[DBG] " + msg, Level::DEBUG);
    }

    void BaseLogger::info(const std::string& msg) {
        log("[INFO] " + msg, Level::INFO);
    }

    void BaseLogger::warn(const std::string& msg) {
        log("[WARN] " + msg, Level::WARNING);
    }

    void BaseLogger::error(const std::string& msg) {
        log("[ERR] " + msg, Level::ERROR);
    }

    void BaseLogger::set_level(Level level) {
        level_ = level;
    }

    Level BaseLogger::get_level() const {
        return level_;
    }

    void BaseLogger::flush() {
        output_.flush();
    }

    void BaseLogger::log(const std::string& msg, Level level) const {
        if (level >= level_) {
            output_ << msg + '\n';
        }
    }

}
