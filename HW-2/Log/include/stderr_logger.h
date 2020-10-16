//
// Created by Osip Chin on 16.10.2020.
//

#ifndef STDERR_LOGGER_H
#define STDERR_LOGGER_H

#include "logger.h"

namespace log {

    class StderrLogger : public BaseLogger {
    public:
        StderrLogger(Level level = Level::Debug);

        void flush() override;
    private:
        void log(const std::string& msg, Level level) override;
    };


}


#endif //STDERR_LOGGER_H
