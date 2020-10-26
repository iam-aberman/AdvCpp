//
// Created by Osip Chin on 16.10.2020.
//

#ifndef BASE_LOGGER_H
#define BASE_LOGGER_H

#include <string>
#include <iostream>

namespace log {

    enum class Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    class BaseLogger {
    public:
        BaseLogger() = delete;
        BaseLogger(const BaseLogger&) = delete;

        virtual ~BaseLogger();

        void debug(const std::string& msg);
        void info(const std::string& msg);
        void warn(const std::string& msg);
        void error(const std::string& msg);

        void set_level(Level level);
        Level get_level() const;

        void flush();

    protected:
        explicit BaseLogger(std::ostream& os, Level level = Level::INFO);

    private:
        void log(const std::string& msg, Level level) const;

        Level level_;
        std::ostream& output_;
    };

}

#endif //BASE_LOGGER_H
