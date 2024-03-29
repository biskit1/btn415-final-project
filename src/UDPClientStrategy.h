#pragma once

#include <windows.networking.sockets.h>

#include "iStrategy.h"

class UDPClientStrategy : public iStrategy {
private:
	bool& connected;
	SOCKET& sock;
	sockaddr_in& SvrAddr;
	sockaddr_in& RespAddr;
	int& RespAddrSize;
public:
	UDPClientStrategy(bool&, SOCKET&, sockaddr_in&, sockaddr_in&, int&);
	bool Setup();
	bool ConnectTCP();
	bool DisconnectTCP();
	bool SetupUDP();
	bool TerminateUDP();
	int SendData(const char*, int);
	int GetData(char*, int);
};

