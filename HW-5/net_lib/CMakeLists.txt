add_library(net source/tmp.cpp source/net_epoll.cpp include/net_exception.h)

target_link_libraries(net tcp)
target_include_directories(net PUBLIC ${CMAKE_SOURCE_DIR}/tcp/include)
target_include_directories(net PUBLIC include)