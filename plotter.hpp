#ifndef PLOTTER_H
#define PLOTTER_H
#include "plt4822.hpp"

#include <csignal>
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>

using std::this_thread::sleep_for;
using namespace std::chrono_literals;

struct Pos {
    double x, y;
    Pos operator+(const Pos& b) const { return {x+b.x, y+b.y}; }
    Pos operator*(double f) const { return {f*x, f*y}; }
    Pos operator-(const Pos& b) const { return {x-b.x, y-b.y}; }
};

std::istream& operator>>(std::istream& in, Pos& pos) {
    return in >> pos.x >> pos.y;
}


double dist(const Pos& a, const Pos& b) {
    return std::hypot(b.x-a.x, b.y-a.y);
}

class Plotter : private plt4822 {
    Pos curr{};
    void setOutput(const Pos& pos);
    public:
    using plt4822::plt4822;
    using plt4822::setPen;
    void gotoPos(const Pos& pos);
};

#endif
