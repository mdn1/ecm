// Header protection
#ifndef CONF_HPP
#define CONF_HPP 1

#include <cstdint>

constexpr uint8_t adapter_nr = 1; // get available adapters with the cmd: ->	ls /dev/*i2c*
constexpr uint8_t i2c_bus_adress_length = 17;
// uint16_t slave_adress = 0x08;

#endif // CONF_HPP