#include "TCPClientStrategy.h"

TCPClientStrategy::TCPClientStrategy(bool& connected, SOCKET& sock, sockaddr_in& SvrAddr)
	: connected(connected),
	sock(sock),
	SvrAddr(SvrAddr)
{}

bool TCPClientStrategy::Setup()
{
	return ConnectTCP();
}

bool TCPClientStrategy::ConnectTCP()
{
	if (connected) {
		return true;
	}
	else {
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == INVALID_SOCKET) {
			return false;
		}
		else {
			if ((connect(sock, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
				closesocket(sock);
				return false;
			}
			else {
				connected = true;
				return true;
			}
		}
	}
}

bool TCPClientStrategy::DisconnectTCP()
{
	if (!connected) {
		return true;
	}
	else {
		if (sock == INVALID_SOCKET) {
			return false;
		}
		else {
			if (closesocket(sock) == 0) {
				sock = INVALID_SOCKET;
				connected = false;
				return true;
			}
			else {
				return false;
			}
		}
	}
}

bool TCPClientStrategy::SetupUDP()
{
	return false;
}

bool TCPClientStrategy::TerminateUDP()
{
	return false;
}

int TCPClientStrategy::SendData(const char* data, int len)
{
	return send(sock, data, len, 0);
}

int TCPClientStrategy::GetData(char* buff, int len)
{
	return recv(sock, buff, len, 0);
}
