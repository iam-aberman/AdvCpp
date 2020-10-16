//
// Created by Osip Chin on 16.10.2020.
//

#ifndef LOG_SINGLETON_H
#define LOG_SINGLETON_H

#include "logger.h"

#include <memory>

namespace log {

    class Logger {
    public:
        static Logger& get();

        std::unique_ptr<BaseLogger>& get_global_logger();
        void set_global_logger(std::unique_ptr<BaseLogger> logger);

    private:
        Logger();

        static Logger instance_;
        std::unique_ptr<BaseLogger> logger_;
    };

}

#endif //LOG_SINGLETON_H
