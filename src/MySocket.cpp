#include "MySocket.h"
#include "TCPClientStrategy.h"
#include "TCPServerStrategy.h"
#include "UDPClientStrategy.h"
#include "UDPServerStrategy.h"

std::string MySocket::GetIPAddr()
{
	return IPAddr;
}

int MySocket::GetPort()
{
	return Port;
}

SocketType MySocket::GetType()
{
	return mySocket;
}

bool MySocket::SetType(SocketType st) {

	bool ret = false;
	//check if things have already been initalized
	if (bConnect == false) {
		mySocket = st;
		ret = true;
	}
	return ret;
}

bool MySocket::SetPortNum(int port)
{
	bool ret = false;
	//check if things have already been initialized
	if (bConnect == false) {
		Port = port;
		SvrAddr.sin_port = htons(Port);
		ret = true;
	}
	return ret;
}


bool MySocket::SetIPAddr(std::string ip)
{
	bool ret = false;
	//check if things have already been initialized
	if (bConnect == false) {
		IPAddr = ip;
		SvrAddr.sin_addr.s_addr = inet_addr(IPAddr.c_str());
		ret = true;
	}
	return ret;
}

MySocket::MySocket(SocketType SType, std::string IP, unsigned int port, ConnectionType CType, unsigned int size)
{
	commStrategy = nullptr;
	WelcomeSocket = INVALID_SOCKET;
	ConnectionSocket = INVALID_SOCKET;
	MaxSize = DEFAULT_SIZE;
	RespAddr = { 0 };
	RespAddrSize = sizeof(RespAddr);
	bConnect = false;

	if (StartWSA()) {
		mySocket = SType;
		connectionType = CType;
		if (size != 0) {
			MaxSize = size;
		}
		Buffer = new char[MaxSize];
		
		SvrAddr.sin_family = AF_INET;

		SetPortNum(port);
		SetIPAddr(IP);

		switch (connectionType) {
		case UDP:
			switch (mySocket) {
			case CLIENT:
				commStrategy = new UDPClientStrategy(bConnect, ConnectionSocket, SvrAddr, RespAddr, RespAddrSize);
				break;
			case SERVER:
				commStrategy = new UDPServerStrategy(bConnect, WelcomeSocket, SvrAddr, RespAddr, RespAddrSize);
				break;
			}
			break;
		case TCP:
			switch (mySocket) {
			case CLIENT:
				commStrategy = new TCPClientStrategy(bConnect, ConnectionSocket, SvrAddr);
				break;
			case SERVER:
				commStrategy = new TCPServerStrategy(bConnect, WelcomeSocket, ConnectionSocket, SvrAddr);
				break;
			}
			break;
		}

		if (commStrategy) {
			bConnect = commStrategy->Setup();
		}
	}
	else {
		mySocket = INVALID;
		connectionType = NONE;
		IPAddr = "";
		Port = 0;
		SvrAddr = { 0 };
		Buffer = nullptr;
	}
}

MySocket::~MySocket()
{
	DisconnectTCP();
	TerminateUDP();
	delete[] Buffer;
	delete commStrategy;
	WSACleanup();
}

bool MySocket::StartWSA()
{
	WSADATA wsa_data;
	bool ret = true;
	if ((WSAStartup(MAKEWORD(2, 2), &wsa_data)) != 0) {
		ret = false;
	}
	return ret;
}

bool MySocket::WSAStarted() {
	return mySocket != INVALID && connectionType != NONE;
}

bool MySocket::ConnectTCP()
{
	return WSAStarted() && commStrategy->ConnectTCP();
}

bool MySocket::DisconnectTCP()
{
	return WSAStarted() && commStrategy->DisconnectTCP();
}

bool MySocket::SetupUDP()
{
	return WSAStarted() && commStrategy->SetupUDP();
}

bool MySocket::TerminateUDP()
{
	return WSAStarted() && commStrategy->TerminateUDP();
}

int MySocket::SendData(const char * data, int numBytes)
{
	int bytesSent = 0;

	if (bConnect && WSAStarted()) {
		bytesSent = commStrategy->SendData(data, numBytes);
	}

	return bytesSent;
}

int MySocket::GetData(char * data)
{
	int bytesWritten = 0;

	if (bConnect && WSAStarted()) {
		bytesWritten = commStrategy->GetData(Buffer, MaxSize);
	}

	if (bytesWritten > 0) {
		memcpy(data, Buffer, bytesWritten);
	}

	return bytesWritten;
}
