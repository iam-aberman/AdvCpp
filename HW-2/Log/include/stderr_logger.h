//
// Created by Osip Chin on 16.10.2020.
//

#ifndef STDERR_LOGGER_H
#define STDERR_LOGGER_H

#include "base_logger.h"
// This is temporary
#include <iostream>

namespace log {

    class StderrLogger : public BaseLogger {
    public:
        explicit StderrLogger(Level level = Level::INFO);

        void flush() override;
    private:
        void log(const std::string& msg, Level level) override;
    };


}


#endif //STDERR_LOGGER_H
