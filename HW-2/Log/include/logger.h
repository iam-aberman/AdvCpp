//
// Created by Osip Chin on 16.10.2020.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <map>

namespace log {

    enum class Level {
        Debug, Info,
        Warning, Error
    };

    class BaseLogger {
    public:
        BaseLogger(Level level = Level::Debug);
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
        static const std::map<Level, std::string> level_to_prefix_;

    private:
        Level level_;
    };

}

#endif //LOGGER_H
