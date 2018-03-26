#pragma once

class PktDef {
	struct Header {
		unsigned int PktCount;
		unsigned char Sleep : 1;
		unsigned char Status : 1;
		unsigned char Drive : 1;
		unsigned char Claw : 1;
		unsigned char Arm : 1;
		unsigned char Ack : 1;
		unsigned char Padding : 2;
		unsigned char Length;
	};

	struct Packet {
		Header Header;
		char* Data;
		char CRC;
	};

	Packet CmdPacket;
};
