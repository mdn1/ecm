//======================================================================================================================
// C O P Y R I G H T
//======================================================================================================================
/// \file       tcp_message.cpp
/// \brief      Implementation of the tcp message class.
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
///
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
// Inclusions
//======================================================================================================================
#include <string>

#include "gpio_board.hpp"
#include "tcp_message.hpp"

//======================================================================================================================
// Type Definitions / Enums / Defines / Macros / Consts
//======================================================================================================================

//======================================================================================================================
// Local Method Prototypes
//======================================================================================================================

//======================================================================================================================
// Variables, Objects
//======================================================================================================================
std::queue<TcpMessage> tcpRxMsgs;

// Getting the mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t dataNotProduced = PTHREAD_COND_INITIALIZER;
pthread_cond_t dataNotConsumed = PTHREAD_COND_INITIALIZER;

//======================================================================================================================
// Local Implementation
//======================================================================================================================

//======================================================================================================================
// API Implementation
//======================================================================================================================

/// ------------------------------------------------------------------------------------------------------------
/// See description in Header file.
/// ------------------------------------------------------------------------------------------------------------
void TcpMessage::setPayload(const std::string &payload)
{
    m_payload = payload;
}

/// ------------------------------------------------------------------------------------------------------------
/// See description in Header file.
/// ------------------------------------------------------------------------------------------------------------
std::string TcpMessage::getPayload()
{
    return m_payload;
}

/// ------------------------------------------------------------------------------------------------------------
/// See description in Header file.
/// ------------------------------------------------------------------------------------------------------------
std::vector<std::string> TcpMessage::splitMsg(const std::string &fullMsg)
{
    std::string msg = fullMsg;
    std::vector<std::string> splittedMsg{};
    size_t pos = 0;
    
    // Iterate as many times as there are delimiter symbols in the string.
    while ((pos = msg.find(tcpMsgDelimiter)) != std::string::npos) 
    {
        // Extract the first sub-string starting at 0 and ending where the delimiter was found.
        splittedMsg.push_back(msg.substr(0, pos));

        // Remove any '\n' at the end of the string if present.
        if (!splittedMsg.back().empty() &&
            splittedMsg.back()[splittedMsg.back().length() - 1] == '\n')
        {
            splittedMsg.back().erase(splittedMsg.back().length() - 1);
        }

        // Remove substring from main string.
        msg.erase(0, pos + tcpMsgDelimiter.length());
    }

    // Add the last string segment to the queue.
    if (pos == std::string::npos)
    {
        splittedMsg.push_back(msg);

        // Remove any '\n' at the end of the string if present.
        if (!splittedMsg.back().empty() && splittedMsg.back()[splittedMsg.back().length()-1] == '\n') {
            splittedMsg.back().erase(splittedMsg.back().length()-1);
        }
    }

    return splittedMsg;
}


/// ------------------------------------------------------------------------------------------------------------
/// See description in Header file.
/// ------------------------------------------------------------------------------------------------------------
parsedMsgType TcpMessage::parseMsg(std::vector<std::string> &msg)
{
    commandType command;
    argumentType argument;
    std::map<parameterType, std::bitset<8>> parameters;

        //******************************************
        //              Parse command
        //******************************************
        // At least 1 word for the command.
        if (msg.size() < 1)
        {
                // No message sent.
                return parsedMsgType(1);
        }

        // Parse first word.
        if (msg[0] == commandControl)
        {
                command = commandType::commandControl;
        }
        else if (msg[0] == commandTest)
        {
                return parsedMsgType(0, commandType::commandTest);
        }
        else if (0)
        {
                // currently not in use
                // command = commandType::XYZ;
        }
        else
        {
                // No recognized command found.
                return parsedMsgType(2);
        }

        //******************************************
        //              Parse argument
        //******************************************
        // At least 1 word for the argument.
        if (msg.size() < 2)
        {
                // No argument found.
                return parsedMsgType(3);
        }

        // Parse the argument
        if (msg[1] == argumentLed)
        {
                argument = argumentType::argumentLed;
        }
        else if (0)
        {
                // currently not in use
                // argument = argumentType::XYZ;
        }
        else
        {
                // No recognized argument found.
                return parsedMsgType(4);
        }

        //******************************************
        //              Parse parameters
        //******************************************
        // At least 1 word for the parameters.
        if (msg.size() < 3)
        {
                // No valid parameter sent.
                return parsedMsgType(5);
        }

        int positionInMsg = 2; // Posistion 0 is the command. Position 1 is the argument.
        size_t pos = 0;

        // Extract all parameters
        while (positionInMsg <= msg.size() - 1)
        {
            if ((pos = msg[positionInMsg].find(tcpMsgParameterDelimiter)) == std::string::npos)
            {
                // Error with parameter format.
                return parsedMsgType(6);
            }

            // Parameter parameterPanel= "--panel" found.
            if (msg[positionInMsg].find(parameterPanel) != std::string::npos)
            {
                std::bitset<8> newPanelValue = gpio_boards::allOff;
                std::string binString = "";

                // Extract binary value from string.
                binString = msg[positionInMsg].substr(pos + tcpMsgParameterDelimiter.size());

                // Convert string to binary number (char).
                newPanelValue = std::stoi(binString, nullptr, 2);

                // Delete binary value "=XXXXXXX".
                msg[positionInMsg].erase(pos);

                // Update the correct led panel.
                if (msg[positionInMsg] == parameterPanel1)
                {
                    parameters.insert({parameterType::parameterPanel1, newPanelValue});
                }
                else if (msg[positionInMsg] == parameterPanel2)
                {
                    parameters.insert({parameterType::parameterPanel2, newPanelValue});
                }
                else if (msg[positionInMsg] == parameterPanel3)
                {
                    parameters.insert({parameterType::parameterPanel3, newPanelValue});
                }
                else if (msg[positionInMsg] == parameterPanel4)
                {
                    parameters.insert({parameterType::parameterPanel4, newPanelValue});
                }
                else if (msg[positionInMsg] == parameterPanel5)
                {
                    parameters.insert({parameterType::parameterPanel5, newPanelValue});
                }
                else if (msg[positionInMsg] == parameterPanel6)
                {
                    parameters.insert({parameterType::parameterPanel6, newPanelValue});
                }
                else if (msg[positionInMsg] == parameterPanel7)
                {
                    parameters.insert({parameterType::parameterPanel7, newPanelValue});
                }
                else if (msg[positionInMsg] == parameterPanel8)
                {
                    parameters.insert({parameterType::parameterPanel8, newPanelValue});
                }
                else
                {
                    return parsedMsgType(6);
                }
            }
            else if (msg[positionInMsg].find(parameterPanelAll) != std::string::npos)
            {
                std::string stateString = "";

                // Extract state value from string.
                stateString = msg[positionInMsg].substr(pos + tcpMsgParameterDelimiter.size());

                // Convert string to binary number (char).
                if (stateString == parameterPanelOn)
                {
                    parameters.insert({parameterType::parameterPanelAll, std::bitset<8>(gpio_boards::allOn)}); 
                }
                else if(stateString == parameterPanelOff)
                {
                    parameters.insert({parameterType::parameterPanelAll, std::bitset<8>(gpio_boards::allOff)}); 
                }
                else
                {
                    // No valid parameter sent.
                    return parsedMsgType(5);
                }

                // Delete binary value "=XXXXXXX".
                msg[positionInMsg].erase(pos);
            }
            else
            {
                return parsedMsgType(5);
            }
            positionInMsg++;
        }

    return parsedMsgType(0, command, argument, parameters);
}