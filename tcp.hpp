//======================================================================================================================
// C O P Y R I G H T
//======================================================================================================================
/// \file       tcp.hpp
/// \brief      TODO
/// \details    

///             Server side:
///             	1. Create a socket (socket())
///             	2. Bind it to a port (bind())
///             	3. Set it to listen state. (listen())
///             	4. Accept a connection (accept())
///             	5. Send and receive data with accepted socket (send() & recv())
///             
///             Client side
///             	1. Create a socket (socket())
///             	2. Establish a connection (connect())
///             	3. Send and recive data (send() & recv())
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
#ifndef TEMPLATE_HPP_
#define TEMPLATE_HPP_ 1


//======================================================================================================================
// Inclusions
//======================================================================================================================
#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<pthread.h> //for threading , link with lpthread

#include"conf.hpp"


//======================================================================================================================
// Type Definitions / Enums / Defines / Macros / Consts
//======================================================================================================================
// struct arg_struct {
//     int socket_desc;
//     pthread_t threadId;
// };


//======================================================================================================================
// Extern Variables
//======================================================================================================================
extern bool shutdownOccured;


//======================================================================================================================
// External Constants
//======================================================================================================================


//======================================================================================================================
// Prototypes
//======================================================================================================================
/// ----------------------------------------------------------------------------------------------------------------
/// <summary>
/// One line description.
/// <para><b>Description</b></para>
///     Multiple line description.
///     Return: U8 status, 0=OK (no error)
///     </summary>
/// <param name='param1'>
///     Description of the first paramter.
///     </param>
/// <param name='param2'>
///     Description of the second paramter.
///     </param>
/// ----------------------------------------------------------------------------------------------------------------
void *startTcpServer(void * arg);


void *connectionHandler(void *socket_desc);


#endif // TEMPLATE_HPP_