#ifndef PLT4822_H
#define PLT4822_H
#include <stdint.h>

enum Channel : uint8_t {CH_A = 0, CH_B=1};
enum Gain : uint8_t {X2 = 0, X1=1};
enum PenState : bool {PENUP=false, PENDOWN=true};

class plt4822 {
    int spifd;
    int pl_value;
    uint8_t pl_pin;
    Gain gain;

    inline int writeSPI(uint16_t word);
public:
    plt4822(const char* device="/dev/spidev0.0", uint8_t pl_pin=5, uint32_t speed=10000);
    plt4822(const plt4822&) = delete;
    ~plt4822();

    void setGain(Gain g);
    void output(Channel ch, uint16_t value);
    void output(uint16_t ch_a, uint16_t ch_b);
    void setPen(PenState s);
};
#endif // PLT4822_H
