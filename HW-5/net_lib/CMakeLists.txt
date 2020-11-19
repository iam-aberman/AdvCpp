add_library(net source/net_epoll.cpp
                source/buffered_connection.cpp
                source/service.cpp)

target_link_libraries(net tcp)
target_include_directories(net PUBLIC ${CMAKE_SOURCE_DIR}/tcp/include)
target_include_directories(net PUBLIC include)