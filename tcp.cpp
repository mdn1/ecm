//======================================================================================================================
// C O P Y R I G H T
//======================================================================================================================
/// \file       tcp.cpp
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
#include <stdio.h>
#include <string.h> 		//strlen
#include <string>			//string typr
#include <stdlib.h> 		//strlen
#include <sys/socket.h>
#include <arpa/inet.h> 		//inet_addr
#include <unistd.h>	   		//write
#include <pthread.h>   		//for threading , link with lpthread
#include <vector>
#include <iostream> 		//Needed for cout

#include "tcp.hpp"


//======================================================================================================================
// Type Definitions / Enums / Defines / Macros / Consts
//======================================================================================================================


//======================================================================================================================
// Local Method Prototypes
//======================================================================================================================


//======================================================================================================================
// Variables, Objects
//======================================================================================================================
bool shutdownOccured = false;
std::string message;
int socket_desc;
int new_socket;
int c;
int *new_sock;
int err = 0;
std::vector<pthread_t> threads;
//----------------------------------------------
// 	// IPv4 AF_INET sockets:
// struct sockaddr_in {
//     short            sin_family;   // e.g. AF_INET, AF_INET6
//     unsigned short   sin_port;     // e.g. htons(3490)
//     struct in_addr   sin_addr;     // see struct in_addr, below
//     char             sin_zero[8];  // zero this if you want to
// };

// struct in_addr {
//     unsigned long s_addr;          // load with inet_pton()
// };

// struct sockaddr {
//     unsigned short    sa_family;    // address family, AF_xxx
//     char              sa_data[14];  // 14 bytes of protocol address
// };
//----------------------------------------------
struct sockaddr_in server, client;

//======================================================================================================================
// Local Implementation
//======================================================================================================================

//======================================================================================================================
// API Implementation
//======================================================================================================================

/// ------------------------------------------------------------------------------------------------------------
/// See description in Header file.
/// ------------------------------------------------------------------------------------------------------------
void *startTcpServer(void *arg)
{
	// Create socket
	//----------------------------------------------
	// Address Family - AF_INET (this is IP version 4) (AF_INET6 for IPV6)
	// Type - SOCK_STREAM (this means connection oriented TCP protocol) (SOCK_DGRAM  for UDP)
	// Protocol - 0 [ or IPPROTO_IP This is IP protocol]
	//----------------------------------------------
	socket_desc = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket\n");
	}

	// Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY; // inet_addr("192.168.111.50"); //inet_addr converts an ip adress to a long format. //alternative: INADDR_ANY;
	server.sin_port = htons(tcpSocketPort);

	// Socket is bound to a local address.
	if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf("bind failed\n");
		return NULL;
	}
	printf("bind done\n");

	// Put socket into listen mode
	listen(socket_desc, 3);

	// Accept an incoming connection
	printf("Waiting for incoming connections...\n");
	c = sizeof(struct sockaddr_in);

	// TCP Thread loop.
	while (shutdownOccured == false)
	{
		// Check if there is any connection awaiting to be accepted.
		new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);

		// Create TCP connection thread only after a connection has been accepted.
		if (new_socket != -1)
		// if (new_socket != EAGAIN && new_socket != EWOULDBLOCK && new_socket >= 0)
		{
			printf("Connection accepted\n");

			char *client_ip = inet_ntoa(client.sin_addr);
			int client_port = ntohs(client.sin_port);
			printf("Client Ip:\t%d\n", *client_ip);
			printf("Client Port:\t%d\n", client_port);

			// Reply to the client
			message = "Hello Client , I have received your connection. But I have to go now, bye\n";
			write(new_socket, message.data(), strlen(message.data()));

			pthread_t sniffer_thread;
			new_sock = (int *)malloc(1);
			*new_sock = new_socket;

			err = pthread_create(&sniffer_thread, NULL, connectionHandler, (void *)new_sock);
			if (err)
			{
				std::cout << "TCP thread creation failed : " << strerror(err);
			}
			else
			{
				std::cout << "TCP thread created with ID : " << sniffer_thread << std::endl;
				threads.emplace_back(sniffer_thread);
			}

			// Now join the thread , so that we dont terminate before the thread
			// pthread_join( sniffer_thread , NULL);
			printf("Handler assigned\n");
		}
		// Either there was a connection problem or simply no client has requested a connection.
		else if (new_socket == -1)
		{
			// No client has requested a connection.
			if (errno == EWOULDBLOCK)
			{
				// printf("No pending connections; sleeping for one second.\n");
				sleep(1);
			}
			// Connection error.
			else
			{
				perror("error when accepting connection");
			}
		}
	}

	// TCP Thread Shutdown requested. Join all TCP handler threads.
	for (pthread_t &th : threads)
	{
		pthread_join(th, NULL);
	}
	threads.clear();

	close(socket_desc);
	printf("TCP socket closed.\n");

	return 0;
}

/// ------------------------------------------------------------------------------------------------------------
/// See description in Header file.
/// ------------------------------------------------------------------------------------------------------------
void *connectionHandler(void *socket_desc)
{
	// Get the socket descriptor
	int sock = *(int *)socket_desc;
	int read_size;
	char client_message[2000];
	std::string message;

	// Send some messages to the client
	message = "Greetings! I am your connection handler\n";
	write(sock, message.data(), strlen(message.data()));
	message = "Now type something and i shall repeat what you type \n";
	write(sock, message.data(), strlen(message.data()));

	// Receive a message from client
	while (shutdownOccured == false)
	{
		read_size = recv(sock, client_message, 2000, 0);

		if (read_size > 0)
		{
			// Send the message back to client
			write(sock, client_message, strlen(client_message));
		}
		// Either there was a connection problem or simply no client sent a msg.
		else if (read_size == 0)
		{
			printf("Client disconnected\n");
			break;
		}
		else if (read_size == -1)
		{
			// No client has sent a message.
			if (errno == EWOULDBLOCK)
			{
				
			}
			// Connection error.
			else
			{
				printf("error when reading from client.\n");
				break;
			}
		}
	}

	fflush(stdout);

	// Free the socket pointer
	free(socket_desc);

	return 0;
}