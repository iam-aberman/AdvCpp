#include "log.h"

int main() {

    log::init_with_stdout_logger(log::Level::WARNING);

    log::debug("This shouldn't show up!");
    log::info("This shouldn't show up x2!");
    log::warn("Some warning!");
    log::error("Some error!");

    log::set_level(log::Level::INFO);
    log::debug("Still shouldn't show up!");
    log::info("Some info!");
    log::warn("Some warning x2!");
    log::error("Some error x2!");

    return 0;
}

