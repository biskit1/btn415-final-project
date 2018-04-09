#include "UDPClientStrategy.h"

UDPClientStrategy::UDPClientStrategy(bool& connected, SOCKET& sock, sockaddr_in& SvrAddr, sockaddr_in& RespAddr, int& RespAddrSize)
	: connected(connected),
	sock(sock),
	SvrAddr(SvrAddr),
	RespAddr(RespAddr),
	RespAddrSize(RespAddrSize)
{}

bool UDPClientStrategy::Setup()
{
	return SetupUDP();
}

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
	if (connected) {
		return true;
	}
	else {
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (sock != INVALID_SOCKET) {
			connected = true;
			return true;
		}
		else {
			return false;
		}
	}
}

bool UDPClientStrategy::TerminateUDP()
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

int UDPClientStrategy::SendData(const char* data, int len)
{
	return sendto(sock, data, len, 0, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr));
}

int UDPClientStrategy::GetData(char* buff, int len)
{
	return recvfrom(sock, buff, len, 0, (struct sockaddr*)&RespAddr, &RespAddrSize);
}
