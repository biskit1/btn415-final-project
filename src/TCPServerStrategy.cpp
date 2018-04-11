#include "TCPServerStrategy.h"

TCPServerStrategy::TCPServerStrategy(bool& connected, SOCKET& WelcomeSock, SOCKET& ConnSock, sockaddr_in& SvrAddr)
	: connected(connected),
	WelcomeSocket(WelcomeSock),
	ConnectionSocket(ConnSock),
	SvrAddr(SvrAddr)
{}

bool TCPServerStrategy::Setup() {
	return ConnectTCP();
}

bool TCPServerStrategy::ConnectTCP() {
	if (connected) {
		return true;
	}
	else {
		WelcomeSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (WelcomeSocket == INVALID_SOCKET) {
			return false;
		}
		else {
			if ((bind(WelcomeSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
				closesocket(WelcomeSocket);
				return false;
			}
			else {
				if (listen(WelcomeSocket, 1) == SOCKET_ERROR) {
					closesocket(WelcomeSocket);
					return false;
				}
				else {
					if ((ConnectionSocket = accept(WelcomeSocket, NULL, NULL)) == SOCKET_ERROR) {
						closesocket(WelcomeSocket);
						return false;
					}
					connected = true;
					return true;
				}
			}
		}
	}
}

bool TCPServerStrategy::DisconnectTCP() {
	if (!connected) {
		return true;
	}
	else {
		if (WelcomeSocket == INVALID_SOCKET || ConnectionSocket == INVALID_SOCKET) {
			return false;
		}
		else {
			if (closesocket(WelcomeSocket) == 0 && closesocket(ConnectionSocket) == 0) {
				WelcomeSocket = INVALID_SOCKET;
				ConnectionSocket = INVALID_SOCKET;
				connected = false;
				return true;
			}
			else {
				return false;
			}
		}
	}
}

bool TCPServerStrategy::SetupUDP() {
	return false;
}

bool TCPServerStrategy::TerminateUDP() {
	return false;
}

int TCPServerStrategy::SendData(const char* data, int len) {
	return send(ConnectionSocket, data, len, 0);
}

int TCPServerStrategy::GetData(char* buff, int len) {
	return recv(ConnectionSocket, buff, len, 0);
}

