
#include "Net.h"
#include <iostream>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")



//#include "Log.h"

using namespace std;

void Net::initialise()
{
	//set up winsock 2.2
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,2),&wsaData) !=0)
	{
		//error!
		//Log::writeToLog("Failed to initialise winsock");
	}
}


/**
Sets up UDP communication
*/
void Net::setupUDP(int port, char * ip)
{
	try
	{
		portNum = port; //port I'll be listening on

		//populate my_addr structure
		my_addr.sin_family = AF_INET; 
		my_addr.sin_port = htons(portNum); 
		my_addr.sin_addr.s_addr = inet_addr(ip);
		memset(my_addr.sin_zero, 0, 8);

		//create UDP socket and bind to my_addr
		sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(sockfd== INVALID_SOCKET)
			//Log::writeToLog("Call to socket() failed");

		if( bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == SOCKET_ERROR)
			//Log::writeToLog("Call to bind() failed");

		//clear out the socket sets
		FD_ZERO(&master);    // clear the master and temp sets
		FD_ZERO(&read_fds);

		//add the listening socket to the master set
		FD_SET(sockfd, &master);

		fdmax = sockfd;
	}
	catch (char* str)
	{
		ostringstream s;
		s<<"ERROR: "<<str<<endl;
		s<<"ERROR CODE: "<<WSAGetLastError()<<endl;
		//Log::writeToLog(s);
	}
	
}



/**
returns number of bytes received
-1 means an error occured.
*/
int Net::receiveData(char* message)
{
	int bytes_received = 0; 	
	
	read_fds = master; // copy master list
	
	int size = sizeof(their_addr);
	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1;

	int result = select(0,&read_fds,NULL,NULL,&tv);
	/*
	s << "result is " << result<<endl;
	Log::writeToLog(s);
	s.str("");
	*/

	if(result==-1)
	{
		s << "Problem with select call, error no. "<<WSAGetLastError();
		//Log::writeToLog(s);
		s.str("");
	}
	if (result>0)
	{
		
			if (FD_ISSET(sockfd, &read_fds)) 
			{ 	
				bytes_received = recvfrom(sockfd,message,BUFFER_SIZE,0,(sockaddr*)&their_addr,&size);
				
				if(bytes_received <= 0)
				{


				}
				if(bytes_received >=0)
					message[bytes_received]='\0';	
				
				
				//ostringstream s;
				s << "bytes_received " << bytes_received << endl;
				if(bytes_received == -1)
				{
					s << "error "<< WSAGetLastError() << endl;
				}
				s << "Listening on " << inet_ntoa(my_addr.sin_addr)<<":"<<ntohs(my_addr.sin_port)<<endl;
				s << "message from " << getSenderIP()<<":"<<getSenderPort()<<" ";
				
				//if(bytes_received != -1)
					//	s << message;

				//Log::writeToLog(s);				
				
			}
		
		
	}		

	return bytes_received;
}


/**
	Send message to destIP:port
*/
void Net::sendData(char* destIP,int port,char* message)
{
	//set up destination address information
	int destPort = port;
	struct sockaddr_in dest_addr; 

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(destPort); 
	dest_addr.sin_addr.s_addr = inet_addr(destIP); 
	memset(dest_addr.sin_zero, '\0',8);
	
	int len, bytes_sent; 
	len = strlen(message);

	/*
	ostringstream s;
	s<<"send to " << destPort << " from " << this->portNum << endl;
	//s <<"message in net.send() is " << message << endl;
	
	Log::writeToLog(s);*/
			
	sendto(sockfd,message,len,0,(struct sockaddr*)&dest_addr,sizeof (struct sockaddr));
}

void Net::error(const char* error)
{
	std::cout<<error<<std::endl;
	std::cout<<WSAGetLastError()<<std::endl;
}

void Net::closeSocket()
{
	closesocket(sockfd);
		
}

char* Net::getSenderIP()
{
	return inet_ntoa(their_addr.sin_addr);
}

int Net::getSenderPort()
{
	return ntohs(their_addr.sin_port);
}
void Net::cleanup()
{
	closeSocket();
	WSACleanup(); 
}