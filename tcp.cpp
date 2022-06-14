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
#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<pthread.h> //for threading , link with lpthread

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
char *message;
int socket_desc , new_socket , c, *new_sock;
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
struct sockaddr_in server , client;


//======================================================================================================================
// Local Implementation
//======================================================================================================================


//======================================================================================================================
// API Implementation
//======================================================================================================================


/// ------------------------------------------------------------------------------------------------------------
/// See description in Header file.
/// ------------------------------------------------------------------------------------------------------------
int startTcpServer()
{
    //Create socket
	//----------------------------------------------
	//Address Family - AF_INET (this is IP version 4) (AF_INET6 for IPV6)
	//Type - SOCK_STREAM (this means connection oriented TCP protocol) (SOCK_DGRAM  for UDP)
	//Protocol - 0 [ or IPPROTO_IP This is IP protocol]
	//----------------------------------------------
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket\n");
    }
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;//inet_addr("192.168.111.50"); //inet_addr converts an ip adress to a long format. //alternative: INADDR_ANY;
    server.sin_port = htons( 8887 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("bind failed\n");
		return 1;
    }
    printf("bind done\n");
     
    //Put socket into listen mode
    listen(socket_desc , 3);
     
    //Accept an incoming connection
    printf("Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);

	while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Connection accepted");

		char *client_ip = inet_ntoa(client.sin_addr);
		int client_port = ntohs(client.sin_port);

		printf("Client Ip:\t%d\n", client_ip);
		printf("Client Port:\t%d\n", client_port);

		//Reply to the client
		message = "Hello Client , I have received your connection. But I have to go now, bye\n";
		write(new_socket , message , strlen(message));


		pthread_t sniffer_thread;
		new_sock = (int*)malloc(1);
		*new_sock = new_socket;
		
		if( pthread_create( &sniffer_thread , NULL ,  connectionHandler , (void*) new_sock) < 0)
		{
			perror("could not create thread");
			return 1;
		}
		
		//Now join the thread , so that we dont terminate before the thread
		//pthread_join( sniffer_thread , NULL);
		puts("Handler assigned");

	}
	
	if (new_socket<0)
	{
		perror("accept failed");
		return 1;
	}

	close(socket_desc);
	printf("Connection closed.\n");
 
    return 0;

}


/// ------------------------------------------------------------------------------------------------------------
/// See description in Header file.
/// ------------------------------------------------------------------------------------------------------------
void *connectionHandler(void *socket_desc)
{
    //Get the socket descriptor
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[2000];
	
	//Send some messages to the client
	message = "Greetings! I am your connection handler\n";
	write(sock , message , strlen(message));
	
	message = "Now type something and i shall repeat what you type \n";
	write(sock , message , strlen(message));
	
	//Receive a message from client
	while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
	{
		//Send the message back to client
		write(sock , client_message , strlen(client_message));
	}
	
	if(read_size == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}
		
	//Free the socket pointer
	free(socket_desc);
	
	return 0;
}