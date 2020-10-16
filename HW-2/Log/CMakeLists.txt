add_library(log_lib source/logger.cpp source/log_singleton.cpp
                    source/stderr_logger.cpp source/stdout_logger.cpp
                    source/file_logger.cpp source/log_utils.cpp)
target_include_directories(log_lib PUBLIC include)