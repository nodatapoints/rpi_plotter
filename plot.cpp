#include "plotter.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <csignal>

const auto dev = "/dev/spidev0.0";
const auto plPin = 5;
const auto freq = 50'000u;

#include <chrono>
#include <thread>

/* FIXME put this into Plotter options */
using std::this_thread::sleep_for;
using namespace std::chrono_literals;

constexpr auto penup_delay = 200ms;
constexpr auto pendown_delay = 200ms;
/* ----------------------------------- */

volatile std::sig_atomic_t signal_status;

int main(int argc, const char *argv[]) {
    std::signal(SIGINT, [](int signal) {
        std::cerr << "\nInterrupt" << std::endl;
        signal_status = signal;
    });

    std::ifstream infile;
    if (argc == 2) {
        infile.open(argv[1]);
        if (!infile) {
            std::cerr << "could not open file: " << argv[1] << std::endl;
            return -1;
        }
    }
    std::istream& input{(infile) ? infile : std::cin};

    Plotter plotter{dev, plPin, freq};

    Pos pos;
    PenState penup{PENUP};
    while (!signal_status && input) {
        std::string line;
        std::getline(input, line);

        if (line.empty()) {
            penup = plotter.setPen(PENUP);
            continue;
        }
        std::istringstream linestream{line};
        linestream >> pos;

        if (penup) {
            penup = plotter.setPen(PENDOWN);
            sleep_for(pendown_delay);
        }

        plotter.gotoPos(pos);
    }

    return signal_status;
}
