#include "plotter.hpp"

namespace {
    constexpr auto speed = 100;  // mm per second
    constexpr auto segment_length = 1.0;  // mm

    constexpr auto ts = segment_length / speed * 1s;
    constexpr auto unit_per_mm = 10.0;
}

void Plotter::setOutput(const Pos& pos) {
    output(
        std::round(pos.x * unit_per_mm),
        std::round(pos.y * unit_per_mm));
}

void Plotter::gotoPos(const Pos& pos) {
    const double d = dist(curr, pos);
    const unsigned nsteps = std::round(d / segment_length);

    for (unsigned i = 0; i < nsteps; ++i) {
        setOutput(curr + (pos-curr)*(d/nsteps)*i );
        sleep_for(ts);
    }
    curr = pos;
}
