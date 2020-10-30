//
// Created by Osip Chin on 30.10.2020.
//
#include <iostream>
#include <unistd.h>
#include "server.h"
#include "connection.h"

using namespace tcp;

int main() {
    std::string addr = "127.0.0.1";
    uint16_t port = 7070;

    int child_pid = ::fork();
    if (child_pid < 0) {
        std::cerr << "Fork failed!";
        return -1;
    }

    if (child_pid > 0) {
        Server server(addr, port);

        Connection server_to_client = server.accept();
        std::cerr << "[PARENT] Connection acquired\n";

        std::string message = "Hello from parent!";
        server_to_client.write(message.data(), message.size());
    } else {
        ::sleep(2);
        Connection client_to_server(addr, port);

        std::string buf(1024, '\0');
        size_t read = client_to_server.read(buf.data(), buf.size());
        std::cout << "[CHILD] Message: " << buf.substr(0, read);
    }

    return 0;
}
