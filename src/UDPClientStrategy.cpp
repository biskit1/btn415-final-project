#include "UDPClientStrategy.h"

UDPClientStrategy::UDPClientStrategy(SOCKET& sock, sockaddr_in& SvrAddr, sockaddr_in& RespAddr, int& RespAddrSize)
	: UDPSocket(sock),
	SvrAddr(SvrAddr),
	RespAddr(RespAddr),
	RespAddrSize(RespAddrSize)
{}

bool UDPClientStrategy::ConnectTCP()
{
	return false;
}

bool UDPClientStrategy::DisconnectTCP()
{
	return false;
}

bool UDPClientStrategy::SetupUDP()
{
	UDPSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	return UDPSocket != INVALID_SOCKET;
}

bool UDPClientStrategy::TerminateUDP()
{
	if (UDPSocket == INVALID_SOCKET) {
		return false;
	}
	else {
		return closesocket(UDPSocket) == 0;
	}
}

int UDPClientStrategy::SendData(const char* data, int len)
{
	return sendto(UDPSocket, data, len, 0, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr));
}

int UDPClientStrategy::GetData(char* buff, int len)
{
	return recvfrom(UDPSocket, buff, len, 0, (struct sockaddr*)&RespAddr, &RespAddrSize);
}
