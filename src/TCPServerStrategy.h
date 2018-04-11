#pragma once

#include <windows.networking.sockets.h>

#include "iStrategy.h"

class TCPServerStrategy : public iStrategy {
private:
	bool& connected;
	SOCKET& WelcomeSocket;
	SOCKET& ConnectionSocket;
	sockaddr_in& SvrAddr;
public:
	TCPServerStrategy(bool&, SOCKET&, SOCKET&, sockaddr_in&);
	bool Setup();
	bool ConnectTCP();
	bool DisconnectTCP();
	bool SetupUDP();
	bool TerminateUDP();
	int SendData(const char*, int);
	int GetData(char*, int);
};

