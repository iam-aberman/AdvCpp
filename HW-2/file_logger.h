//
// Created by Osip Chin on 16.10.2020.
//

#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H

#include "logger.h"

#include <fstream>

namespace log {

    class FileLogger : public BaseLogger {
    public:
        FileLogger(const std::string& filename);
        ~FileLogger() override;

        void flush() override;
    private:
        void log(const std::string& msg, Level level) override;

        std::ofstream output_file_;
    };

}


#endif //FILE_LOGGER_H
