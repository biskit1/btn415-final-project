#include "UDPServerStrategy.h"

UDPServerStrategy::UDPServerStrategy(SOCKET& sock, sockaddr_in& SvrAddr, sockaddr_in& RespAddr, int& RespAddrSize)
	: sock(sock),
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
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock != INVALID_SOCKET) {
		if ((bind(sock, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) != SOCKET_ERROR) {
			return true;
		}
		else {
			closesocket(sock);
		}
	}
	return false;
}

bool UDPServerStrategy::TerminateUDP()
{
	if (sock != INVALID_SOCKET && closesocket(sock) == 0) {
		sock = INVALID_SOCKET;
		return true;
	}
	else {
		return false;
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
