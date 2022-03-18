// Header protection
#ifndef PCF8574_HPP
#define PCF8574_HPP 1

#include <cstdint>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "conf.hpp"

//  pcf8574 class declaration  -----------------------------------------------//

class pcf8574
{
public:
    pcf8574(char *, uint16_t); // i2cbuslocation, address
    ~pcf8574();
    void setByte(unsigned char);
    void setBit(int, int); // pin,value
    unsigned char getByte();
    uint8_t getBit(uint8_t); // pin
    uint16_t getAddress() { return address; }
    void setAddress(uint16_t address) { this->address = address; }
    char *geti2c_bus_location() { return i2c_bus_location; }
    void seti2c_bus_location(char *i2c_bus_location) { strcpy(this->i2c_bus_location, i2c_bus_location); }

private:
    uint16_t address;
    unsigned char state;
    char i2c_bus_location[i2c_bus_adress_length];
};

#endif // PCF8574_HPP