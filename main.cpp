#include "process.h"

#include <iostream>
#include <thread>
#include <chrono>

int main() {

    Process process("date");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    process.closeStdin();

    std::string output(100, '\0');
    size_t output_size = process.read(output.data(), 99);
    output.resize(output_size);

    std::cout << "Process #date output: ";
    std::cout << output << std::endl;

    return 0;
}

