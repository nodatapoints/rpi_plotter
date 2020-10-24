#include "plotter.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <csignal>

const auto dev = "/dev/spidev0.0";
const auto plPin = 5;
const auto freq = 50'000u;

volatile std::sig_atomic_t signal_status;

int main(int argc, const char *argv[]) {
    std::signal(SIGINT, [](int signal) {
        std::cerr << "abort" << std::endl;
        signal_status = signal;
    });

    if (argc == 2) {
        std::ifstream input{argv[1]};
        if (!input) {
            std::cerr << "could not open file: " << argv[1] << std::endl;
        }
        std::cin.rdbuf(input.rdbuf());
    }

    Plotter plotter{dev, plPin, freq};

    Pos pos{};
    PenState penup = PENUP;
    while (!signal_status && std::cin) {
        std::string line;
        std::getline(std::cin, line);

        if (line.empty()) {
            penup = plotter.setPen(PENUP);
            continue;
        }
        std::istringstream linestream{line};
        linestream >> pos;

        if (penup)
            penup = plotter.setPen(PENDOWN);

        plotter.gotoPos(pos);
    }

    return signal_status;
}
