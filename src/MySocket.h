#pragma once

#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

#include<iostream>

enum SocketType {CLIENT, SERVER};
enum ConnectionType {TCP, UDP};
//should be as big as the biggest possible thing we can receive.
const int DEFAULT_SIZE = 255;

class MySocket {
	char * Buffer;
	SOCKET WelcomeSocket;
	SOCKET ConnectionSocket;
	struct sockaddr_in SvrAddr;
	struct sockaddr_in RespAddr;
	SocketType mySocket;
	std::string IPAddr;
	int Port;
	ConnectionType connectionType;
	bool bConnect;
	int MaxSize;

public:
	MySocket(SocketType, std::string, unsigned int, ConnectionType, unsigned int);
	~MySocket();
	bool StartWSA();
	bool ConnectTCP();
	bool DisconnectTCP();
	bool SetupUDP();
	bool TerminateUDP();
	int SendData(const char *, int);
	int GetData(char *);
	std::string GetIPAddr();
	bool SetIPAddr(std::string);
	bool SetPortNum(int);
	int GetPort();
	SocketType GetType();
	bool setType(SocketType);
};