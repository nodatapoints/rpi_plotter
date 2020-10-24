#include "plt4822.hpp"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

const uint8_t mode = SPI_MODE_0;
const uint8_t bits = 8;

plt4822::plt4822(const char* device, uint8_t pl_pin, uint32_t speed)
        : pl_pin(pl_pin), gain(X1) {
    
    spifd = open(device, O_WRONLY);
    ioctl(spifd, SPI_IOC_WR_MODE, &mode);
    ioctl(spifd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(spifd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

    char buf[50];
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    size_t n = sprintf(buf, "%d\n", pl_pin);
    write(fd, buf, n);
    close(fd);

    sprintf(buf, "/sys/class/gpio/gpio%d/direction", pl_pin);

    while (access(buf, W_OK) < 0);
    fd = open(buf, O_WRONLY);
    write(fd, "out\n", 4);
    close(fd);

    sprintf(buf, "/sys/class/gpio/gpio%d/value", pl_pin);

    while (access(buf, W_OK) < 0);
    pl_value = open(buf, O_WRONLY);
}

plt4822::~plt4822() {
    disable();
    close(spifd);
    close(pl_value);

    char buf[4];
    int fd = open("/sys/class/gpio/unexport", O_WRONLY);
    size_t n = sprintf(buf, "%d\n", pl_pin);
    write(fd, buf, n);
    close(fd);
}

inline int plt4822::writeSPI(uint16_t word) {
    return write(spifd, &word, 2);
}

void plt4822::setGain(Gain a) {
    gain = a;
}

PenState plt4822::setPen(PenState s) {
    write(pl_value, (s) ? "0\n" : "1\n", 2);
    return s;
}

void plt4822::output(Channel ch, uint16_t value) {
    // all worth it for the following three lines
    value &= 0x0FFF; // use 12 bit
    value |= (ch << 15) | (gain << 13) | (1 << 12); // set flags
    writeSPI( (value>>8) | ((0xFF&value)<<8) ); // swap bytes
}

void plt4822::output(uint16_t ch_a, uint16_t ch_b) {
    output(CH_A, ch_a);
    output(CH_B, ch_b);
}

void plt4822::disable() {
    writeSPI(0b0000'0000'0000'0000);
    writeSPI(0b0000'0000'1000'0000);
}
