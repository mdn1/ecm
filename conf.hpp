// Header protection
#ifndef CONF_HPP
#define CONF_HPP 1

#include <cstdint>
#include <string>

const std::string i2cBusPort = "/dev/i2c-1";
constexpr uint8_t maxi2cBusPortLength = 17;
//constexpr char i2cBusPort[maxi2cBusPortLength] = "/dev/i2c-1"; // get available adapters with the cmd: ->	ls /dev/*i2c*
//constexpr uint8_t i2cAdapterNr = 1; 
// uint16_t slave_adress = 0x08;

#endif // CONF_HPP