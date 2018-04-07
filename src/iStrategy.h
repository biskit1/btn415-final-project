#pragma once

class iStrategy {
public:
	virtual bool ConnectTCP() = 0;
	virtual bool DisconnectTCP() = 0;
	virtual bool SetupUDP() = 0;
	virtual bool TerminateUDP() = 0;
	virtual int SendData(const char*, int) = 0;
	virtual int GetData(char*, int) = 0;
};