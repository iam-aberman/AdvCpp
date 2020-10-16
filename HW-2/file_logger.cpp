//
// Created by Osip Chin on 16.10.2020.
//

#include "file_logger.h"

#include <fstream>

namespace log {

    FileLogger::FileLogger(const std::string& filename) : BaseLogger() {
        output_file_ = std::ofstream(filename);
        if (!output_file_) {
            throw std::invalid_argument("bad_fstream");
        }
    }

    FileLogger::~FileLogger() {
        output_file_.close();
    }

    void FileLogger::flush() {
        output_file_.flush();
    }

    void FileLogger::log(const std::string& msg, Level level) {
        if (level >= get_level()) {
            output_file_ << level_to_prefix_.at(level) << msg + '\n';
        }
    }

}
