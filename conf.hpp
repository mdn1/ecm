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
const std::string tcpMsgDelimiter = " "; // Symbol used as delimiter in the tcpMsgs.
const std::string tcpMsgParameterDelimiter = "="; // Symbol used as delimiter in the tcpMsgs parameter.
const int tcpMsgMaxSize = 2000; // Max size of bytes for a tcp message.
constexpr uint8_t maxi2cBusPortLength = 17;
constexpr int tcpSocketPort = 8881;
//constexpr char i2cBusPort[maxi2cBusPortLength] = "/dev/i2c-1"; // get available adapters with the cmd: ->	ls /dev/*i2c*
//constexpr uint8_t i2cAdapterNr = 1; 
// uint16_t slave_adress = 0x08;

/* In total there are 8 LED Panels with 6 LEDs each.
 * 
 * To control the outputs send this string using the TCP protocol:
 * [COMMAND] [ARGUMENT1] [PARAMETER1] [PARAMETER2] [...] ...
 * SojuzControl --led --panel1=00XXXXXX --panel2=00XXXXXX ...
 */

const std::string commandControl = "SojuzControl";

const std::string argumentLed = "--led";

const std::string parameterPanel = "--panel";
const std::string parameterPanel1 = "--panel1";
const std::string parameterPanel2 = "--panel2";
const std::string parameterPanel3 = "--panel3";
const std::string parameterPanel4 = "--panel4";
const std::string parameterPanel5 = "--panel5";
const std::string parameterPanel6 = "--panel6";
const std::string parameterPanel7 = "--panel7";
const std::string parameterPanel8 = "--panel8";

enum class commandType
{
    commandControl = 0,
};

enum class argumentType
{
    argumentLed = 0,
};

#endif // CONF_HPP