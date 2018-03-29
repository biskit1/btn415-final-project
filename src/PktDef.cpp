#include <cstring>

#include "PktDef.h"

PktDef::PktDef() {
	CmdPacket.Header = {};
	CmdPacket.Data = nullptr;
	CmdPacket.CRC = 0;
}

PktDef::PktDef(char * buffer) {

	memcpy(&CmdPacket.Header.PktCount, &buffer[0], sizeof(Packet::Header.PktCount));

	//setup for bitshifting from 4th byte assuming stored in LSB
	char * ptr = &buffer[4];
	CmdPacket.Header.Sleep = *ptr & 1;
	CmdPacket.Header.Status = (*ptr >> 1) & 1;
	CmdPacket.Header.Drive = (*ptr >> 2) & 1;
	CmdPacket.Header.Claw = (*ptr >> 3) & 1;
	CmdPacket.Header.Arm = (*ptr >> 4) & 1;
	CmdPacket.Header.Ack = (*ptr >> 5) & 1;

	//set padding to 0
	CmdPacket.Header.Padding = 0;

	memcpy(&CmdPacket.Header.Length, &buffer[5], sizeof(Packet::Header.Length));

	SetBodyData(&buffer[6], CmdPacket.Header.Length - HEADERSIZE - sizeof(Packet::CRC));
	memcpy(&CmdPacket.CRC, &buffer[CmdPacket.Header.Length - sizeof(Packet::CRC)], sizeof(Packet::CRC));
}

char * PktDef::GenPacket()
{
	
	//+ 1 for charcacter
	int size = HEADERSIZE + CmdPacket.Header.Length + sizeof(Packet::CRC);

	//creating a buffer to hold exactly the packet
	RawBuffer = new char[size];

	//memcpy from the beginning
	memcpy(&RawBuffer[0], &CmdPacket.Header.PktCount, sizeof(Packet::Header.PktCount));

	//making a pointer to the start of the flags
	char *ptr = (char *) &CmdPacket.Header.PktCount + sizeof(Packet::Header.PktCount);

	memcpy(&RawBuffer[4], ptr, sizeof(char));
	memcpy(&RawBuffer[5], &CmdPacket.Header.Length, sizeof(Packet::Header.Length));
	memcpy(&RawBuffer[6], &CmdPacket.Data, CmdPacket.Header.Length - HEADERSIZE - sizeof(Packet::CRC));

	//calculating the next place in the array to get data from
	int next = HEADERSIZE + CmdPacket.Header.Length;
	memcpy(&RawBuffer[next], &this->CmdPacket.CRC, sizeof(Packet::CRC));

	return RawBuffer;
}

void PktDef::SetBodyData(char * data, int size)
{
	CmdPacket.Data = new char[size];
	memcpy(&CmdPacket.Data, &data[0], size);
}
