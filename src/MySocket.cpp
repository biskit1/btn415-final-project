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

bool MySocket::setType(SocketType st){

	bool ret = false;
	//check if things have already been initalized
	if (bConnect == false) {
		mySocket = st;
		ret = true;
	}
	return false;
}

bool MySocket::SetPortNum(int port)
{
	bool ret = false;
	//check if things have already been initialized
	if (bConnect == false) {
		Port = port;
		ret = true;
	}
	else {
		//error message?
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
	StartWSA();
	mySocket = SType;
	connectionType = CType;
	IPAddr = IP;
	Port = port;
	if (size != 0)
		MaxSize = size;
	else
		MaxSize = DEFAULT_SIZE;
	//Note that the constructor should put servers in conditions to either accept connections (if TCP), or to receive messages(if UDP).
	//unsure of what it means, assuming it is calling the setup based on MS2.cpp
	if (connectionType == UDP) {
		SetupUDP();
	}
	else if (connectionType == TCP) {
		ConnectTCP();
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
	//run WSAStartup, if it is not 0 (fails) set the return to false
	//potential enhancement for later: check if WSA has already been startede
	if ((WSAStartup(MAKEWORD(2, 2), &wsa_data)) != 0) {
		//Print("Could not start DLLs");
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
