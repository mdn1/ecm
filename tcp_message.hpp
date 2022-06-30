//======================================================================================================================
// C O P Y R I G H T
//======================================================================================================================
/// \file       tcp_message.hpp
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

#include "conf.hpp"

//======================================================================================================================
// Type Definitions / Enums / Defines / Macros / Consts
//======================================================================================================================
// Class forward declaration
class TcpMessage;

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
    


};

#endif // TCP_MESSAGE_HPP_