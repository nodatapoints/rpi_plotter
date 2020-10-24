#include "plt4822.hpp"
#include <iostream>
#include <csignal>

namespace {
    volatile std::sig_atomic_t signal_status;
}

void abort_plotter(int signal) {
    std::cerr << "abort" << std::endl;
    signal_status = signal;
}


int main() {
    std::signal(SIGINT, abort_plotter);
    plt4822 plotter{"/dev/spidev0.0", 5, 1000u};

    plotter.setGain(X2);
    while (!signal_status)
        plotter.output(CH_B, 42);

    return 0;
}
