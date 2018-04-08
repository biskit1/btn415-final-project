#include "MySocket.h"

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

bool MySocket::SetType(SocketType st){

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
		ret = true;
	}
	return ret;
}

MySocket::MySocket(SocketType SType, std::string IP, unsigned int port, ConnectionType CType, unsigned int size)
{
	if (StartWSA()) {
		mySocket = SType;
		connectionType = CType;
		IPAddr = IP;
		Port = port;
		if (size != 0) {
			MaxSize = size;
		}
		else {
			MaxSize = DEFAULT_SIZE;
		}
		Buffer = new char[MaxSize];
		ConnectionSocket = INVALID_SOCKET;
		WelcomeSocket = INVALID_SOCKET;

		SvrAddr.sin_family = AF_INET;
		SvrAddr.sin_port = htons(Port);
		SvrAddr.sin_addr.s_addr = inet_addr(IPAddr.c_str());

		RespAddr = { 0 };

		switch (connectionType) {
		case UDP:
			SetupUDP();
			break;

		case TCP:
			ConnectTCP();
			break;
		}
	}
	else {
		mySocket = INVALID;
		IPAddr = "";
		connectionType = NONE;
		Port = 0;
		MaxSize = 0;
		ConnectionSocket = INVALID_SOCKET;
		WelcomeSocket = INVALID_SOCKET;
		SvrAddr = { 0 };
		RespAddr = { 0 };
		Buffer = nullptr;
		bConnect = false;
	}
}

MySocket::~MySocket()
{
	delete[] Buffer;
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

bool MySocket::ConnectTCP()
{
	//dummy for ConnectTCP, TBD Later
	return false;
}

bool MySocket::DisconnectTCP()
{
	//dummy for DisconnectTCP, TBD Later
	return false;
}

bool MySocket::SetupUDP()
{
	//dummy for setupUDP, TBD Later
	return false;
}

bool MySocket::TerminateUDP()
{
	//dummy for TerminateUDP, TBD Later
	return false;
}

int MySocket::SendData(const char * data, int numBytes)
{
	//dummy for SendData, TBD Later
	//will be split into two parts

	//UDP


	//TCP
	return 0;
}

int MySocket::GetData(char * data)
{
	//dummy for GetData, TBD Later
	//this will be split into two parts:

	//TCP


	//UDP




	return 0;
}
