//======================================================================================================================
// C O P Y R I G H T
//======================================================================================================================
/// \file       tcp_message.cpp
/// \brief      TODO
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
// Inclusions
//======================================================================================================================
#include <string>

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