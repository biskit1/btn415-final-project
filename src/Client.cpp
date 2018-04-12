
#include <thread>
#include <iostream>
#include <string>
#include "MySocket.h"
#include "PktDef.h"

bool ExeComplete;

//order of arguements: IP, Com, Tel 
int main(int argc, char *argv[])
{
	ExeComplete = false;

	std::string IP = argv[1];
	int ComPort = std::stoi(argv[2]);
	int TelPort = std::stoi(argv[3]);

	//spawn command thread and detach
	//spawn telemetry thread and detach

	while (!ExeComplete) {
		//???
	}

	return 1;
}
