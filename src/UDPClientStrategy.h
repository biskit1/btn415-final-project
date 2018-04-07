#pragma once

#include <windows.networking.sockets.h>

#include "iStrategy.h"

class UDPClientStrategy : public iStrategy {
private:
	SOCKET& UDPSocket;
	sockaddr_in& SvrAddr;
	sockaddr_in& RespAddr;
	int& RespAddrSize;
public:
	UDPClientStrategy(SOCKET&, sockaddr_in&, sockaddr_in&, int&);
	bool ConnectTCP();
	bool DisconnectTCP();
	bool SetupUDP();
	bool TerminateUDP();
	int SendData(const char*, int);
	int GetData(char*, int);
};

