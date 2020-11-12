#include "shared_map.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    using namespace shmem;

    {
        SharedMap<int, std::string> int_to_string(BlockCount(16), BlockSize(128));

        pid_t child = ::fork();
        if (child < 0) {
            std::cerr << "Fork failed!\n";
            return -1;
        }

        if (child > 0) {
            std::cout << "[PARENT] Insert {7, \"This string contains more than 24 characters!\"}\n";
            int_to_string.insert(7, "This string contains more than 24 characters!");
            ::waitpid(child, nullptr, 0);
        } else {
            ::sleep(1);
            std::cout << "[CHILD] SharedMap[7] = \"" +
                         int_to_string.get(7) + "\"\n";
            return 0;
        }
        int_to_string.delete_map();
    }

    {
        SharedMap<std::string, int> string_to_int(BlockCount(16), BlockSize(128));

        pid_t child = ::fork();
        if (child < 0) {
            std::cerr << "Fork failed!\n";
            return -1;
        }

        if (child > 0) {
            ::sleep(1);
            std::cout << "[PARENT] SharedMap[\"This string contains more "
                         "than 24 characters!\"] = " <<
                         string_to_int.get("This string contains more than 24 characters!") <<
                         "\n";
            ::waitpid(child, nullptr, 0);
        } else {
            std::cout << "[CHILD] Insert {\"This string contains more than 24 characters!\", 7}\n";
            string_to_int.insert("This string contains more than 24 characters!", 7);
            return 0;
        }
        string_to_int.delete_map();
    }

    return 0;
}
