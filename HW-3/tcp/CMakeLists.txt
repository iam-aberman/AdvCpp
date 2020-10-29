add_library(tcp source/descriptor.cpp
                source/connection.cpp source/server.cpp)
target_include_directories(tcp PUBLIC include)