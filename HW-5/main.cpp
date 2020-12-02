#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "service.h"
#include "connection.h"
#include "example_listener.h"


int main(int argc, char* argv[]) {

    std::string address = "127.0.0.1";
    int port = 7070;

    net::Service service;
    auto listener = std::make_shared<ExampleListener>();
    service.set_listener(listener);
    service.open(address, port);

    pid_t fork = ::fork();
    if (fork < 0) {
        std::cerr << "Fork failed!\n";
        return -1;
    }

    if (fork == 0) {
        tcp::Connection to_server(address, port);

        ::sleep(2);

        std::string msg = "message";
        to_server.write(msg.data(), msg.size());

        ::sleep(2);
        std::string buf(1024, '\0');
        {
            size_t read = to_server.read(buf.data(), buf.size());
            std::cout << buf.substr(0, read) << std::endl;
        }

        ::sleep(2);
        msg = "advanced C++";
        to_server.write(msg.data(), msg.size());

        ::sleep(2);
        {
            size_t read = to_server.read(buf.data(), buf.size());
            std::cout << buf.substr(0, read) << std::endl;
        }

        to_server.close();
        return 0;
    } else {
        try {
            service.run();
        } catch (const std::exception& e) {}

        ::waitpid(fork, nullptr, 0);
    }

    return 0;
}
