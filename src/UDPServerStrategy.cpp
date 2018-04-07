#include "UDPServerStrategy.h"

UDPServerStrategy::UDPServerStrategy(SOCKET& sock, sockaddr_in& SvrAddr, sockaddr_in& RespAddr, int& RespAddrSize)
	: UDPSocket(sock),
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
	UDPSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (UDPSocket != INVALID_SOCKET) {
		if ((bind(UDPSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) != SOCKET_ERROR) {
			return true;
		}
		else {
			closesocket(UDPSocket);
		}
	}
	return false;
}

bool UDPServerStrategy::TerminateUDP()
{
	if (UDPSocket == INVALID_SOCKET) {
		return false;
	}
	else {
		return closesocket(UDPSocket) == 0;
	}
}

int UDPServerStrategy::SendData(const char* data, int len)
{
	return sendto(UDPSocket, data, len, 0, (struct sockaddr*)&RespAddr, sizeof(RespAddr));
}

int UDPServerStrategy::GetData(char* buff, int len)
{
	return recvfrom(UDPSocket, buff, len, 0, (struct sockaddr*)&RespAddr, &RespAddrSize);
}
