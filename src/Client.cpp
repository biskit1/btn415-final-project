
#include <thread>
#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include "MySocket.h"
#include "PktDef.h"
#include "Telemetry.h"

bool ExeComplete;
std::string makeHex(char * pkt);
void telemetryThread(std::string Ip, int TelPort);

//order of arguements: IP, Com, Tel 
int main(int argc, char *argv[])
{
	ExeComplete = false;
	if (argc != 4) {
		std::cout << "Please enter the IP Address, the Command Port, and the Telemetry Port in that order." << std::endl;
		return 1;
	}
	else {
		std::string IP = argv[1];
		int ComPort = std::stoi(argv[2]);
		int TelPort = std::stoi(argv[3]);

		//spawn command thread and detach
		//spawn telemetry thread and detach
		std::thread Tel(telemetryThread, IP, TelPort);

		while (!ExeComplete) {
			//???
		}
	}

	return 0;
}

std::string makeHex(char * pkt) {
	std::string hexString;
	std::stringstream stream;
	for (int i = 0; i < 12; i++) {
		stream << std::hex << (int)pkt[i];
		if (i != 11) {
			stream << ", ";
		}
	}
	hexString = stream.str();
	return hexString;
}


void telemetryThread(std::string Ip, int TelPort) {
	//all Telemetry Packets are of size 12
	std::ofstream ofs("Telemetry.txt", std::ofstream::out);
	MySocket socket(SocketType::CLIENT, Ip, TelPort, ConnectionType::TCP, 12);
	if (socket.ConnectTCP()) {
		char rx_buffer[255];
		int size;
		bool valid;
		while (!ExeComplete) {
			size = socket.GetData(rx_buffer);
			if (size > 0) {
				PktDef TelPkt(rx_buffer);
				if (size != TelPkt.GetLength()) {
					std::cout << "ERROR: Packet recieved is not of the same size!" << std::endl;
				}
				else {
					if (!TelPkt.CheckCRC(rx_buffer, TelPkt.GetLength())) {
						std::cout << "ERROR: CRC does not match!";
					}
					else {
						if (TelPkt.GetCmd() != STATUS) {
							std::cout << "ERROR: Command is not Status!";
						}
						else {
							Telemetry tel(TelPkt.GetBodyData(), TelPkt.GetPktCount(), makeHex(rx_buffer));
							//std::cout << tel.toString(MULTI);
							ofs << tel.toString(MULTI);
						}
					}

				}
			}
		}
	}
	ofs.close();
}
