//
// Created by Osip Chin on 16.10.2020.
//

#ifndef STDOUT_LOGGER_H
#define STDOUT_LOGGER_H

#include "logger.h"

namespace log {

    class StdoutLogger : public BaseLogger {
    public:
        StdoutLogger();

        void flush() override;
    private:
        void log(const std::string& msg, Level level) override;
    };

}


#endif //STDOUT_LOGGER_H
