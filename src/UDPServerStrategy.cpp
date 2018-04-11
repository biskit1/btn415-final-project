#include "UDPServerStrategy.h"

UDPServerStrategy::UDPServerStrategy(bool& connected, SOCKET& sock, sockaddr_in& SvrAddr, sockaddr_in& RespAddr, int& RespAddrSize)
	: connected(connected),
	sock(sock),
	SvrAddr(SvrAddr),
	RespAddr(RespAddr),
	RespAddrSize(RespAddrSize)
{}

bool UDPServerStrategy::Setup()
{
	return SetupUDP();
}

bool UDPServerStrategy::ConnectTCP()
{
	return false;
}

bool UDPServerStrategy::DisconnectTCP()
{
	return false;
}

bool UDPServerStrategy::SetupUDP()
{
	if (connected) {
		return true;
	}
	else {
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (sock != INVALID_SOCKET) {
			if ((bind(sock, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) != SOCKET_ERROR) {
				connected = true;
				return true;
			}
			else {
				closesocket(sock);
			}
		}
		return false;
	}
}

bool UDPServerStrategy::TerminateUDP()
{
	if (!connected) {
		return true;
	}
	else {
		if (sock != INVALID_SOCKET && closesocket(sock) == 0) {
			sock = INVALID_SOCKET;
			connected = false;
			return true;
		}
		else {
			return false;
		}
	}
}

int UDPServerStrategy::SendData(const char* data, int len)
{
	return sendto(sock, data, len, 0, (struct sockaddr*)&RespAddr, sizeof(RespAddr));
}

int UDPServerStrategy::GetData(char* buff, int len)
{
	return recvfrom(sock, buff, len, 0, (struct sockaddr*)&RespAddr, &RespAddrSize);
}
