#pragma once

#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

#include<iostream>

enum SocketType {CLIENT, SERVER, INVALID};
enum ConnectionType {TCP, UDP, NONE};
const int DEFAULT_SIZE = 7;

class MySocket {
	char * Buffer;
	SOCKET WelcomeSocket;
	SOCKET ConnectionSocket;
	struct sockaddr_in SvrAddr;
	struct sockaddr_in RespAddr;
	int RespAddrSize;
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
	bool CloseTCPSocket(SOCKET& socket);
	int GetPort();
	SocketType GetType();
	bool SetType(SocketType);
};
