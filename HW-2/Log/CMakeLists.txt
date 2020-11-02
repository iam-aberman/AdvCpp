add_library(log_lib source/base_logger.cpp
                    source/log_singleton.cpp
                    source/log.cpp)

target_include_directories(log_lib PUBLIC include)
