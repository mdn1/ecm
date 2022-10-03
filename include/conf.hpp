//======================================================================================================================
// C O P Y R I G H T
//======================================================================================================================
/// \file       conf.hpp
/// \brief      File containing all configuration parameters for the entire application.
/// \details    Refer to each section depending on the purpose of the config.
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

//******************************************
//                   I2C
//******************************************
const std::string i2cBusPort = "/dev/i2c-1";
constexpr uint8_t maxi2cBusPortLength = 17;


//******************************************
//                   TCP
//******************************************
constexpr int tcpSocketPort = 8881;
const std::string tcpMsgDelimiter = " ";                // Symbol used as delimiter in the tcpMsgs.
const std::string tcpMsgParameterDelimiter = "=";       // Symbol used as delimiter in the tcpMsgs parameter.
const int tcpMsgMaxSize = 2000;                         // Max size of bytes for a tcp message.


/* In total there are 8 LED Panels with 6 LEDs each.
 * 
 * To control the outputs send a string using the TCP protocol according to:
 * [COMMAND] [ARGUMENT1] [PARAMETER1] [PARAMETER2] [...] ...
 * 
 * Example:
 * For controlling as many panels as desired. Only add the panels you wish to update.
 *          sojuzControl --led --panel1=00XXXXXX --panel2=00XXXXXX ...
 * For testing all the LEDs:
 *          sojuzTest
 * For turning on/off all LEDs  at one:
 *          sojuzControl --led --all=on
 *          sojuzControl --led --all=off
 */

// TCP msg commands
const std::string commandControl = "sojuzControl";
const std::string commandTest = "sojuzTest";
// TCP msg arguments
const std::string argumentLed = "--led";
// TCP msg parametes
const std::string parameterPanel = "--panel";
const std::string parameterPanel1 = "--panel1";
const std::string parameterPanel2 = "--panel2";
const std::string parameterPanel3 = "--panel3";
const std::string parameterPanel4 = "--panel4";
const std::string parameterPanel5 = "--panel5";
const std::string parameterPanel6 = "--panel6";
const std::string parameterPanel7 = "--panel7";
const std::string parameterPanel8 = "--panel8";
const std::string parameterPanelAll = "--all";
const std::string parameterPanelOn = "on";
const std::string parameterPanelOff = "off";


//******************************************
//                   MISC
//******************************************
const int repetitionsForOutputTest = 3;                 // Ho many on-off repetitions should occur during output test routine.
const int delayLedJumpForOutputTestMs = 100;


#endif // CONF_HPP