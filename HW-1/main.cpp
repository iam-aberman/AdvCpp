#include "process.h"

#include <iostream>
#include <thread>
#include <chrono>

int main() {

    {
        Process process("date", {"date"});
        std::this_thread::sleep_for(std::chrono::seconds(2));
        process.closeStdin();

        std::string output(100, '\0');
        size_t output_size = process.read(output.data(), 100);
        output.resize(output_size);

        std::cout << "Process #date output: ";
        std::cout << output << std::endl;
    }

    {
        const size_t arg_len = 12;

        Process process("echo", {"echo", "AdvancedC++!"});
        std::string output(12, '\0');
        process.readExact(output.data(), arg_len);

        std::cout << "Process #echo output: ";
        std::cout << output << std::endl;
    }

    return 0;
}

