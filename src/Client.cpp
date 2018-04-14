
#include <thread>
#include <iostream>
#include <fstream> 
#include <string>
#include "MySocket.h"
#include "PktDef.h"

bool ExeComplete;

//order of arguements: IP, Com, Tel 
int main(int argc, char *argv[])
{
	ExeComplete = false;
	if(argc != 4){
		std::cout << "Please enter the IP Address, the Command Port, and the Telemetry Port in that order." << std::endl;
		return 1;
	}
	else{
		std::string IP = argv[1];
		int ComPort = std::stoi(argv[2]);
		int TelPort = std::stoi(argv[3]);

		//spawn command thread and detach
		//spawn telemetry thread and detach

		while (!ExeComplete) {
			//???
		}
	}

	return 0;
}


void telemetryThread(std::string Ip, int TelPort) {
	//all Telemetry Packets are of size 12
	MySocket socket(SocketType::CLIENT, Ip, TelPort, ConnectionType::TCP, 7);
	if (socket.ConnectTCP()) {
		char rx_buffer[255];
		int size;
		bool valid;
		int num = 0;
		while (!ExeComplete) {
			size = socket.GetData(rx_buffer);
			if (size > 0) {
				PktDef TelPkt(rx_buffer);
				if (size != TelPkt.GetLength) {
					std::cout << "ERROR: Packet recieved is not of the same size!" << std::endl;
				}
				else {
					if (!TelPkt.CheckCRC(rx_buffer, TelPkt.GetLength())) {
						std::cout << "ERROR: CRC does not match!";
					}
					else {
						if (TelPkt.GetCmd != STATUS) {
							std::cout << "ERROR: Command is not Status!";
						}
						else {
							TeltoFile(TelPkt.GetBodyData(), num);
							num++;
						}
					}

				}
			}
		}
	}
}

void TeltoFile(char * body, int num) {
	std::string msg;
	std::ofstream ofs("Telemetry.txt", std::ofstream::out);
	short int sonarData;
	short int armData;
	bool driveFlag;
	bool armUp;
	bool armDown;
	bool clawOpen;
	bool clawClosed;
	memcpy(&sonarData, &body[0], sizeof(sonarData));
	memcpy(&armData, &body[2], sizeof(armData));
	char *ptr = &body[4];
	driveFlag = *ptr & 1;
	armUp = (*ptr >> 1) & 1;
	armDown = (*ptr >> 2) & 1;
	clawOpen = (*ptr >> 3) & 1;
	clawClosed = (*ptr >> 4) & 1;
	ofs << "Packet: " + num;
	ofs << "Sonar Reading: " +std::to_string(sonarData) << std::endl;
	ofs << "Arm Reading: " + std::to_string(armData) << std::endl;
	if (driveFlag) {
		ofs << "Drive flag: DRIVING" << std::endl;
	}
	else {
		ofs << "Drive flag: STOPPED" << std::endl;
	}
	if (armUp) {
		ofs << "Arm: Up" << std::endl;
	}
	else if (armDown) {
		ofs << "Arm: Down" << std::endl;
	}
	if (clawOpen) {
		ofs << "Claw: Open" << std::endl;
	}
	else if (clawClosed) {
		ofs << "Claw: Closed" << std::endl;
	}

}
