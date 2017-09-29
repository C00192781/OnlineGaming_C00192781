#ifndef _NET_H_
#define _NET_H_
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <sstream>


class Net
{
private:
    int sockfd; //socket file descriptor
	int new_fd; //used to create new socket for new connection 

	sockaddr_in their_addr; //use to store remote address info

	sockaddr_in my_addr; //used to store my address info
	fd_set master;
	int fdmax;
	fd_set read_fds; // temp file descriptor list for select()	
	
	//for the log
	std::ostringstream s;

public:
	static const int BUFFER_SIZE=100;

    void setupUDP(int port);
	void setupUDP(int port, char * ip);
    virtual void sendData(char* ip,int port,char* message);
	int receiveData(char* message);
	void initialise();
	void closeSocket();
	void cleanup();
	void Net::error(const char* error);
	char* Net::getSenderIP();
	int Net::getSenderPort();
	

	int portNum;
};

#endif  