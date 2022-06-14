//======================================================================================================================
// C O P Y R I G H T
//======================================================================================================================
/// \file       conf.hpp
/// \brief      File containing all configuration parameters for the entire application.
/// \details    TODO
/// \author     maintained by: Mario D. Nevola
///
/// \copyright  Copyright (c) 2022 by Universität Stuttgart. All rights reserved. \n
///             The reproduction, distribution and utilization of this file as well as the communication of its \n
///             contents to others without express authorization is prohibited. Offenders will be held liable for \n
///             the payment of damages. \n
///             All rights reserved in the event of the grant of a patent, utility model or design. \n
///             \b Disclaimer: Any modification or usage outside of the intended purpose is not under authors liability. \n
///             \b Usage: Further use of source code files or code snippets is under full liability of the user.
//======================================================================================================================


//======================================================================================================================
// Header Protection
//======================================================================================================================
#ifndef CONF_HPP
#define CONF_HPP 1


//======================================================================================================================
// Inclusions
//======================================================================================================================
#include <cstdint>
#include <string>


//======================================================================================================================
// Type Definitions / Enums / Defines / Macros / Consts
//======================================================================================================================
const std::string i2cBusPort = "/dev/i2c-1";
constexpr uint8_t maxi2cBusPortLength = 17;
//constexpr char i2cBusPort[maxi2cBusPortLength] = "/dev/i2c-1"; // get available adapters with the cmd: ->	ls /dev/*i2c*
//constexpr uint8_t i2cAdapterNr = 1; 
// uint16_t slave_adress = 0x08;


#endif // CONF_HPP