#include "MySocket.h"
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
	WelcomeSocket = INVALID_SOCKET;
	ConnectionSocket = INVALID_SOCKET;
	MaxSize = DEFAULT_SIZE;
	RespAddr = { 0 };
	RespAddrSize = sizeof(RespAddr);
	bConnect = false;

	if (StartWSA()) {
		commStrategy = nullptr;
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
				commStrategy = new UDPClientStrategy(ConnectionSocket, SvrAddr, RespAddr, RespAddrSize);
				break;
			case SERVER:
				commStrategy = new UDPServerStrategy(WelcomeSocket, SvrAddr, RespAddr, RespAddrSize);
				break;
			}
			break;
		case TCP:
			ConnectTCP();
			break;
		}

		if (commStrategy) {
			bConnect = commStrategy->Setup();
		}
	}
	else {
		commStrategy = nullptr;
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
	delete[] Buffer;
	delete commStrategy;
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
	switch (connectionType) {
	case TCP:
		if (bConnect) {
			return true;
		}
		else {
			switch (mySocket) {
			case CLIENT:
				ConnectionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if (ConnectionSocket == INVALID_SOCKET) {
					return false;
				}
				else {
					if ((connect(ConnectionSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
						closesocket(ConnectionSocket);
						return false;
					}
					else {
						bConnect = true;
						return bConnect;
					}
				}
				break;
			case SERVER:
				WelcomeSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if (WelcomeSocket == INVALID_SOCKET) {
					return false;
				}
				else {
					if ((bind(WelcomeSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
						closesocket(WelcomeSocket);
						bConnect = false;
						return bConnect;
					}
					else {
						if (listen(WelcomeSocket, 1) == SOCKET_ERROR) {
							closesocket(WelcomeSocket);
							bConnect = false;
							return bConnect;
						}
						else {
							if ((ConnectionSocket = accept(WelcomeSocket, NULL, NULL)) == SOCKET_ERROR) {
								closesocket(WelcomeSocket);
								bConnect = false;
								return bConnect;
							}
							bConnect = true;
							return bConnect;
						}
					}
				}
				break;
			default:
				return false;
			}
		}
		break;
	case UDP:
		return false;
		break;
	default:
		return false;
	}
}

bool MySocket::DisconnectTCP()
{
	switch (connectionType) {
	case TCP:
		if (bConnect) {
			switch (mySocket) {
			case CLIENT:
				return CloseTCPSocket(ConnectionSocket);
				break;
			case SERVER:
				return CloseTCPSocket(ConnectionSocket) && CloseTCPSocket(WelcomeSocket);
				break;
			default:
				return false;
			}
		}
		else {
			return false;
		}
		break;
	case UDP:
		return false;
		break;
	default:
		return false;
	}
}

bool MySocket::CloseTCPSocket(SOCKET& socket) {
	if (socket == INVALID_SOCKET) {
		return false;
	}
	else {
		if (closesocket(socket) == 0) {
			socket = INVALID_SOCKET;
			bConnect = false;
			return true;
		}
		else {
			return false;
		}
	}
}

bool MySocket::SetupUDP()
{
	switch (connectionType) {
	case TCP:
		return false;
		break;
	case UDP:
		if (!bConnect && WSAStarted()) {
			bConnect = commStrategy->SetupUDP();
		}
		return bConnect;
		break;
	}
}

bool MySocket::TerminateUDP()
{
	switch (connectionType) {
	case TCP:
		return false;
		break;
	case UDP:
		if (bConnect && WSAStarted() && commStrategy->TerminateUDP()) {
			bConnect = false;
			return true;
		}
		else {
			return false;
		}
	}
}

int MySocket::SendData(const char * data, int numBytes)
{
	int bytesSent = 0;

	if (bConnect) {
		switch (connectionType) {
		case TCP:
			switch (mySocket) {
			case CLIENT:
				bytesSent = send(ConnectionSocket, data, numBytes, 0);
				break;
			case SERVER:
				bytesSent = send(ConnectionSocket, data, numBytes, 0);
				break;
			}
			break;
		case UDP:
			if (WSAStarted()) {
				bytesSent = commStrategy->SendData(data, numBytes);
			}			
			break;
		}
	}

	return bytesSent;
}

int MySocket::GetData(char * data)
{
	int bytesWritten = 0;
	if (bConnect) {
		switch (connectionType) {
		case TCP:
			switch (mySocket) {
			case CLIENT:
				bytesWritten = recv(ConnectionSocket, Buffer, MaxSize, 0);
				break;
			case SERVER:
				bytesWritten = recv(ConnectionSocket, Buffer, MaxSize, 0);
				break;
			}
			break;
		case UDP:
			if (WSAStarted()) {
				bytesWritten = commStrategy->GetData(Buffer, MaxSize);
			}
			break;
		}
	}

	if (bytesWritten > 0) {
		memcpy(data, Buffer, bytesWritten);
	}

	return bytesWritten;
}
