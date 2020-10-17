//
// Created by Osip Chin on 16.10.2020.
//

#ifndef BASE_LOGGER_H
#define BASE_LOGGER_H

#include <string>
#include <map>

namespace log {

    enum class Level {
        DEBUG, INFO,
        WARNING, ERROR
    };

    class BaseLogger {
    public:
        explicit BaseLogger(Level level = Level::INFO);
        virtual ~BaseLogger() = default;

        void debug(const std::string& msg);
        void info(const std::string& msg);
        void warn(const std::string& msg);
        void error(const std::string& msg);

        void set_level(Level level);
        Level get_level() const;

        virtual void flush() = 0;

    protected:
        virtual void log(const std::string& msg, Level level) = 0;

    private:
        Level level_;
    };

}

#endif //BASE_LOGGER_H
