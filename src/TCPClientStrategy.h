#pragma once

#include <windows.networking.sockets.h>

#include "iStrategy.h"

class TCPClientStrategy : public iStrategy {
private:
	bool& connected;
	SOCKET& sock;
	sockaddr_in& SvrAddr;
public:
	TCPClientStrategy(bool&, SOCKET&, sockaddr_in&);
	bool Setup();
	bool ConnectTCP();
	bool DisconnectTCP();
	bool SetupUDP();
	bool TerminateUDP();
	int SendData(const char*, int);
	int GetData(char*, int);
};

