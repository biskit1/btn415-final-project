#pragma once

#include <windows.networking.sockets.h>

#include "iStrategy.h"

class UDPServerStrategy : public iStrategy {
private:
	SOCKET& sock;
	sockaddr_in& SvrAddr;
	sockaddr_in& RespAddr;
	int& RespAddrSize;
public:
	UDPServerStrategy(SOCKET&, sockaddr_in&, sockaddr_in&, int&);
	bool Setup();
	bool ConnectTCP();
	bool DisconnectTCP();
	bool SetupUDP();
	bool TerminateUDP();
	int SendData(const char*, int);
	int GetData(char*, int);
};

