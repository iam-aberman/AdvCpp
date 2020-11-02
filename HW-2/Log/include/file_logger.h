//
// Created by Osip Chin on 16.10.2020.
//

#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H

#include "base_logger.h"

#include <fstream>
#include <stdexcept>

namespace log {

    class FileLogger : public BaseLogger {
    public:
        FileLogger() = delete;
        explicit FileLogger(const std::string& filename,
                            Level level = Level::INFO) : output_file_(filename),
                                                         BaseLogger(output_file_, level) {
            if (!output_file_) {
                throw std::runtime_error("bad_file");
            }
        }

    private:
        std::ofstream output_file_;
    };

}


#endif //FILE_LOGGER_H
