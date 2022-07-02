//======================================================================================================================
// C O P Y R I G H T
//======================================================================================================================
/// \file       tcp_message.hpp
/// \brief      Header of the tcp message class.
/// \details    In total there are 8 LED Panels with 6 LEDs each.
///             To control the outputs send a string using the TCP protocol according to:
///                     [COMMAND] [ARGUMENT1] [PARAMETER1] [PARAMETER2] [...] ...
///             Example:
///             For controlling as many panels as desired. Only add the panels you wish to update.
///                      sojuzControl --led --panel1=00XXXXXX --panel2=00XXXXXX ...
///               For testing all the LEDs:
///                      sojuzTest
///               For turning on/off all LEDs  at one:
///                      sojuzControl --led --all=on
///                      sojuzControl --led --all=off
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
#ifndef TCP_MESSAGE_HPP_
#define TCP_MESSAGE_HPP_ 1

//======================================================================================================================
// Inclusions
//======================================================================================================================
#include <pthread.h> //for threading , link with lpthread
#include <queue>
#include <string.h> //strlen
#include <string>   //string typr
#include <vector>
#include <map>
#include <bitset>

#include "conf.hpp"

//======================================================================================================================
// Type Definitions / Enums / Defines / Macros / Consts
//======================================================================================================================
// Class forward declaration
class TcpMessage;

enum class commandType
{
    commandEmpty = 0,
    commandControl = 1,
    commandTest = 2,
};

enum class argumentType
{
    argumentEmpty = 0,
    argumentLed = 1,
};

enum class parameterType
{
    parameterEmpty = 0,
    parameterPanel1 = 1,
    parameterPanel2 = 2,
    parameterPanel3 = 3,
    parameterPanel4 = 4,
    parameterPanel5 = 5,
    parameterPanel6 = 6,
    parameterPanel7 = 7,
    parameterPanel8 = 8,
    parameterPanelAll = 9,
};

struct parsedMsgType
{
    int parsingError = 0;
    commandType command = commandType::commandEmpty;
    argumentType argument = argumentType::argumentEmpty;
    std::map<parameterType, std::bitset<8>> parameters; // Vector containing Parameter and values together.

    parsedMsgType(int newParsingErrors = 0,
                  commandType newCommand = commandType::commandEmpty,
                  argumentType newArgument = argumentType::argumentEmpty,
                  std::map<parameterType, std::bitset<8>> newParameters = std::map<parameterType, std::bitset<8>>{})
    {
        parsingError = newParsingErrors;
        command = newCommand;
        argument = newArgument;
        parameters = newParameters;
    }
};

//======================================================================================================================
// Extern Variables
//======================================================================================================================
// Shared queue
extern std::queue<TcpMessage> tcpRxMsgs;

// Getting the mutex
extern pthread_mutex_t mutex;
extern pthread_cond_t dataNotProduced;
extern pthread_cond_t dataNotConsumed;

//======================================================================================================================
// External Constants
//======================================================================================================================

//======================================================================================================================
// Prototypes
//======================================================================================================================
class TcpMessage
{
private:
    std::string m_payload;

public:
    TcpMessage(const std::string &payload)
    {
        m_payload = payload;
    }

    void setPayload(const std::string &payload);

    std::string getPayload();

    static std::vector<std::string> splitMsg(const std::string &fullMsg);
    static parsedMsgType parseMsg(std::vector<std::string> &msg);
};

#endif // TCP_MESSAGE_HPP_