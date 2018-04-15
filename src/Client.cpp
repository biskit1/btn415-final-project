
#include <thread>
#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include "MySocket.h"
#include "PktDef.h"
#include "Telemetry.h"

#define TELPKTSIZE 12

bool ExeComplete;
std::string makeHex(char * pkt, int size);
void telemetryThread(std::string Ip, int TelPort);
void StartCSI(std::string ip, int port);

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
		std::thread CommandThread(StartCSI, IP, ComPort);
		CommandThread.detach();
		//spawn telemetry thread and detach
		std::thread Tel(telemetryThread, IP, TelPort);
		Tel.detach();

		while (!ExeComplete) {
			//???
		}
	}

	return 0;
}

std::string makeHex(char * pkt, int size) {
	std::string hexString;
	std::stringstream stream;
	for (int i = 0; i < size; i++) {
		stream << std::hex << (int)pkt[i];
		if (i != size - 1) {
			stream << ", ";
		}
	}
	hexString = stream.str();
	return hexString;
}

void telemetryThread(std::string Ip, int TelPort) {
	std::ofstream ofs("Telemetry.txt", std::ofstream::out);
	MySocket socket(SocketType::CLIENT, Ip, TelPort, ConnectionType::TCP, TELPKTSIZE);
	if (socket.ConnectTCP()) {
		char rx_buffer[255];
		int size;
		while (!ExeComplete) {
			size = socket.GetData(rx_buffer);
			if (size > 0) {
				PktDef TelPkt(rx_buffer);
				if (size != TelPkt.GetLength()) {
					std::cout << "ERROR: Packet recieved is not of the expected size!" << std::endl;
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
							Telemetry tel(TelPkt.GetBodyData(), TelPkt.GetPktCount(), makeHex(rx_buffer, TelPkt.GetLength()));
							ofs << tel.toString(MULTI);
						}
					}
				}
			}
		}
	}
	ofs.close();
}

void StartCSI(std::string ip, int port) {
	std::ofstream ofs("Client_Output.txt");
	if (!ofs.is_open())
		std::cout << "ERROR:  Failed to open Client_Output.txt file" << std::endl;
	MySocket ComSocket(SocketType::CLIENT, ip, port, ConnectionType::TCP, 100);
	// if 3 way handshake was successfull 
	if (ComSocket.ConnectTCP()) {

		int cmdSelection = -1;
		int pktCount = 0;

		MotorBody DriveCmd;
		ActuatorBody myAction;
		PktDef TestPkt;

		char *ptr;
		char rxBuffer[100];
		int bytesReceived;
		bool loop = true;

		// keep sending packets until user req to send sleep 
		while (loop) {
			//query the user to get required info to form a packet
			std::cout << "Select a command from the following options: " << std::endl;
			// display options until user inputs a good selection
			do {
				std::cout << "1. DRIVE" << std::endl;
				std::cout << "2. CLAW" << std::endl;
				std::cout << "3. ARM" << std::endl;
				std::cout << "4. SLEEP" << std::endl;

				std::cin >> cmdSelection;

				switch (cmdSelection) {
				case 1:
					TestPkt.SetCmd(DRIVE);
					std::cout << "Select a direction to drive: " << std::endl;
					do {
						std::cout << "1. Forward\n2. Backward\n3. Right\n4. Left" << std::endl;
						std::cin >> cmdSelection;
						switch (cmdSelection) {
							//set motorbody
						case 1:
							DriveCmd.Direction = cmdSelection;
							std::cout << "Input number of seconds to run command for: " << std::endl;
							std::cin >> cmdSelection;
							while (cmdSelection <= 0) {
								std::cout << "Bad input - please input number of seconds to run command for (input > 0): " << std::endl;
								std::cin >> cmdSelection;
							}
							DriveCmd.Duration = cmdSelection;
							break;
						case 2:
							DriveCmd.Direction = cmdSelection;
							std::cout << "Input number of seconds to run command for: " << std::endl;
							std::cin >> cmdSelection;
							while (cmdSelection <= 0) {
								std::cout << "Bad input - please input number of seconds to run command for (input > 0): " << std::endl;
								std::cin >> cmdSelection;
							}
							DriveCmd.Duration = cmdSelection;
							break;
						case 3:
							DriveCmd.Direction = cmdSelection;
							std::cout << "Input number of seconds to run command for: " << std::endl;
							std::cin >> cmdSelection;
							while (cmdSelection <= 0) {
								std::cout << "Bad input - please input number of seconds to run command for (input > 0): " << std::endl;
								std::cin >> cmdSelection;
							}
							DriveCmd.Duration = cmdSelection;
							break;
						case 4:
							DriveCmd.Direction = cmdSelection;
							std::cout << "Input number of seconds to run command for: " << std::endl;
							std::cin >> cmdSelection;
							while (cmdSelection <= 0) {
								std::cout << "Bad input - please input number of seconds to run command for (input > 0): " << std::endl;
								std::cin >> cmdSelection;
							}
							DriveCmd.Duration = cmdSelection;
							break;
						default:
							std::cout << "Bad input - please select a direction from the following options (input 1, 2, 3 or 4): " << std::endl;
							cmdSelection = -1;
						}
					} while (cmdSelection == -1);
					TestPkt.SetBodyData((char*)&DriveCmd, 2);
					TestPkt.SetPktCount(++pktCount);
					TestPkt.CalcCRC();
					ptr = TestPkt.GenPacket();
					ComSocket.SendData(ptr, TestPkt.GetLength());
					ofs << "Raw packet data to transmit: " << std::endl << makeHex(ptr, TestPkt.GetLength()) << std::endl;
					ofs << "Transmitting Packet..." << std::endl;
					break;
				case 2:
					TestPkt.SetCmd(CLAW);
					std::cout << "Select a motion: " << std::endl;
					do {
						std::cout << "1. Open\n2. Close" << std::endl;
						std::cin >> cmdSelection;
						switch (cmdSelection) {
						case 1:
							myAction.Action = OPEN;
							TestPkt.SetBodyData((char *)&myAction, 1);
							TestPkt.SetPktCount(++pktCount);
							TestPkt.CalcCRC();
							ptr = TestPkt.GenPacket();
							ComSocket.SendData(ptr, TestPkt.GetLength());
							ofs << "Raw packet data to transmit: " << std::endl << makeHex(ptr, TestPkt.GetLength()) << std::endl;
							ofs << "Transmitting Packet..." << std::endl;
							break;
						case 2:
							myAction.Action = CLOSE;
							TestPkt.SetBodyData((char *)&myAction, 1);
							TestPkt.SetPktCount(++pktCount);
							TestPkt.CalcCRC();
							ptr = TestPkt.GenPacket();
							ComSocket.SendData(ptr, TestPkt.GetLength());
							ofs << "Raw packet data to transmit: " << std::endl << makeHex(ptr, TestPkt.GetLength()) << std::endl;
							ofs << "Transmitting Packet..." << std::endl;
							break;
						default:
							std::cout << "Bad input - please select a motion from the following options (input 1 or 2): " << std::endl;
							cmdSelection = -1;
						}
					} while (cmdSelection == -1);
					break;
				case 3:
					TestPkt.SetCmd(ARM);
					std::cout << "Select a motion: " << std::endl;
					do {
						std::cout << "1. Up\n2. Down" << std::endl;
						std::cin >> cmdSelection;
						switch (cmdSelection) {
						case 1:
							myAction.Action = UP;
							TestPkt.SetBodyData((char *)&myAction, 1);
							TestPkt.SetPktCount(++pktCount);
							TestPkt.CalcCRC();
							ptr = TestPkt.GenPacket();
							ComSocket.SendData(ptr, TestPkt.GetLength());
							ofs << "Raw packet data to transmit: " << std::endl << makeHex(ptr, TestPkt.GetLength()) << std::endl;
							ofs << "Transmitting Packet..." << std::endl;
							break;
						case 2:
							myAction.Action = DOWN;
							TestPkt.SetBodyData((char *)&myAction, 1);
							TestPkt.SetPktCount(++pktCount);
							TestPkt.CalcCRC();
							ptr = TestPkt.GenPacket();
							ComSocket.SendData(ptr, TestPkt.GetLength());
							ofs << "Raw packet data to transmit: " << std::endl << makeHex(ptr, TestPkt.GetLength()) << std::endl;
							ofs << "Transmitting Packet..." << std::endl;
							break;
						default:
							std::cout << "Bad input - please select a motion from the following options (input 1 or 2): " << std::endl;
							cmdSelection = -1;
						}
					} while (cmdSelection == -1);
					break;
				case 4:
					TestPkt.SetCmd(SLEEP);
					ptr = nullptr;
					TestPkt.SetBodyData(ptr, 0);
					TestPkt.SetPktCount(++pktCount);
					TestPkt.CalcCRC();
					ptr = TestPkt.GenPacket();
					ComSocket.SendData(ptr, TestPkt.GetLength());
					ofs << "Raw packet data to transmit: " << std::endl << makeHex(ptr, TestPkt.GetLength()) << std::endl;
					ofs << "Transmitting Packet..." << std::endl;
					loop = false;
					break;
				default:
					std::cout << "Bad input - please select a command from the following options (input 1, 2, 3 or 4)" << std::endl;
					cmdSelection = -1;
				}
			} while (cmdSelection == -1);
			bytesReceived = ComSocket.GetData(rxBuffer);
			PktDef RxPkt(rxBuffer);
			if (RxPkt.CheckCRC(rxBuffer, bytesReceived)) {
				if (RxPkt.GetAck() && RxPkt.GetCmd() != UNKNOWN && RxPkt.GetCmd() != STATUS) {
					std::cout << "Ackknowledgement Received- Packet delivered successfully" << std::endl;
					ofs << "Ackknowledgement Received- Packet delivered successfully" << std::endl;
					ofs << "Raw packet data: " << std::endl;
					ofs << makeHex(rxBuffer, RxPkt.GetLength()) << std::endl;
					pktCount = RxPkt.GetPktCount();
				}
				else {
					std::cout << "Packet delivery was unsuccessful" << std::endl;
					ofs << "Packet delivery was unsuccessful" << std::endl;
					ofs << "Raw packet data: " << std::endl;
					ofs << makeHex(rxBuffer, RxPkt.GetLength()) << std::endl;
					if (RxPkt.GetCmd() == UNKNOWN && RxPkt.GetAck() == 0) {
						std::cout << "Data received in body: " << RxPkt.GetBodyData() << std::endl;
						ofs << "Data received in body: " << RxPkt.GetBodyData() << std::endl;
					}
					loop = true;
				}
			}
			else {
				std::cout << "Received corrupt data. Discarding and proceeding..." << std::endl;
				ofs << "Received corrupt data. Discarding and proceeding..." << std::endl;
				loop = true;
			}
		}
		if (ComSocket.DisconnectTCP()) {
			ExeComplete = true;
		}
		else {
			std::cout << "Failed to disconnect socket - terminating thread..." << std::endl;
			ofs << "Failed to disconnect socket - terminating thread..." << std::endl;
		}
	}
	else {
		std::cout << "Problem establishing connection" << std::endl;
		ofs << "Problem establishing connection" << std::endl;
	}
}
