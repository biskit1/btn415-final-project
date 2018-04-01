#include "PktDef.h"
#include <cstring>

PktDef::PktDef() {
	CmdPacket.Header = {};
	CmdPacket.Data = nullptr;
	CmdPacket.CRC = 0;
}

PktDef::PktDef(char * buffer) {

	memcpy(&CmdPacket.Header.PktCount, &buffer[0], sizeof(Packet::Header.PktCount));

	//setup for bitshifting from 4th byte assuming stored in LSB
	char * ptr = &buffer[4];
	CmdPacket.Header.Drive = *ptr & 1;
	CmdPacket.Header.Status = (*ptr >> 1) & 1;
	CmdPacket.Header.Sleep = (*ptr >> 2) & 1;
	CmdPacket.Header.Arm = (*ptr >> 3) & 1;
	CmdPacket.Header.Claw = (*ptr >> 4) & 1;
	CmdPacket.Header.Ack = (*ptr >> 5) & 1;

	//set padding to 0
	CmdPacket.Header.Padding = 0;

	memcpy(&CmdPacket.Header.Length, &buffer[5], sizeof(Packet::Header.Length));

	SetBodyData(&buffer[6], CmdPacket.Header.Length - HEADERSIZE - sizeof(Packet::CRC));
	memcpy(&CmdPacket.CRC, &buffer[CmdPacket.Header.Length - sizeof(Packet::CRC)], sizeof(Packet::CRC));
}

char * PktDef::GenPacket()
{

	//creating a buffer to hold exactly the packet
	RawBuffer = new char[CmdPacket.Header.Length];

	//memcpy from the beginning
	memcpy(&RawBuffer[0], &CmdPacket.Header.PktCount, sizeof(Packet::Header.PktCount));

	//making a pointer to the start of the flags
	char *ptr = (char *) &CmdPacket.Header.PktCount + sizeof(Packet::Header.PktCount);

	memcpy(&RawBuffer[4], ptr, sizeof(char));
	memcpy(&RawBuffer[5], &CmdPacket.Header.Length, sizeof(Packet::Header.Length));
	memcpy(&RawBuffer[6], CmdPacket.Data, CmdPacket.Header.Length - HEADERSIZE - sizeof(Packet::CRC));
	memcpy(&RawBuffer[CmdPacket.Header.Length - sizeof(Packet::CRC)], &CmdPacket.CRC, sizeof(Packet::CRC));

	return RawBuffer;
}

void PktDef::SetBodyData(char * data, int size)
{
	CmdPacket.Data = new char[size];
	memcpy(CmdPacket.Data, data, size);
	CmdPacket.Header.Length = HEADERSIZE + size + sizeof(Packet::CRC);
}

CmdType PktDef::GetCmd()
{
	CmdType ret = UNKNOWN;
	int numbits = countFlags();
	//Checking Bits to determine the active type
	if(numbits == 1){
		if (CmdPacket.Header.Drive == 1) {
			ret = DRIVE;
		}
		else if (CmdPacket.Header.Status == 1){
			ret = STATUS;
		}
		else if (CmdPacket.Header.Sleep == 1) {
			ret = SLEEP;
		}
		else if (CmdPacket.Header.Arm == 1) {
			ret = ARM;
		}
		else if (CmdPacket.Header.Claw == 1) {
			ret = CLAW;
		}
	}
	//if the ACK flag is on as well as another flag, it is an ACK
	else if(numbits == 2 && GetAck()){
		ret = ACK;
	}

	return ret;
}

bool PktDef::GetAck()
{
	return CmdPacket.Header.Ack;
}

int PktDef::GetLength()
{
	return CmdPacket.Header.Length;
}

char * PktDef::GetBodyData()
{
	return CmdPacket.Data;
}

int PktDef::GetPktCount()
{
	return CmdPacket.Header.PktCount;
}

int PktDef::countFlags()
{
	int count = 0;
	
	unsigned char* headerFlags = (unsigned char*)&CmdPacket.Header.PktCount + sizeof(CmdPacket.Header.PktCount);

	for (int i = 0; i < 6; i++) {
		if ((*headerFlags >> i) & 1 == 1) {
			count += 1;
		}
	}

	return count;
}

void PktDef::clearFlag(const CmdFlag& flag) {
	unsigned char* headerFlags = (unsigned char*)&CmdPacket.Header.PktCount + sizeof(CmdPacket.Header.PktCount);
	unsigned char ackValue = CmdPacket.Header.Ack;

	memset(headerFlags, 0, 1);

	if (flag != ALL) {
		CmdPacket.Header.Ack = ackValue;
	}
}

void PktDef::SetPktCount(int count)
{
	CmdPacket.Header.PktCount = count;
}

//A set function that sets the packets command flag based on the CmdType
void PktDef::SetCmd(const CmdType& cmd)
{

	if (cmd != ACK) {
		clearFlag(ALL);
	}
	
	switch (cmd) {
	case DRIVE:
		// set pkt cmd flag
		CmdPacket.Header.Drive = 1;
		break;
	case STATUS:
		CmdPacket.Header.Status = 1;
		break;
	case SLEEP:
		// set pkt cmd flag
		CmdPacket.Header.Sleep = 1;
		break;
	case ARM:
		// set pkt cmd flag
		CmdPacket.Header.Arm = 1;
		break;
	case CLAW:
		// set pkt cmd flag
		CmdPacket.Header.Claw = 1;
		break;
	case ACK:
		// set pkt cmd flag
		CmdPacket.Header.Ack = 1;
		break;
	case NACK:
		// set pkt cmd flag
		CmdPacket.Header.Ack = 0;
		break;
	}
}

//a function that calculates the CRC and sets the objects packet CRC parameter.
void PktDef::CalcCRC() {
	char len = GetLength();
	int pktCount = GetPktCount();
	int sizeBody = len - HEADERSIZE - sizeof(Packet::CRC);
	int bufferSize = sizeof(pktCount) + sizeof(char) + sizeof(len) + sizeBody;
	char* bodyPtr = GetBodyData();

	int count = 0;

	char *myBuffer = new char[bufferSize];
	memcpy(&myBuffer[0], &pktCount, sizeof(pktCount));
	char* ptr = (char*)&CmdPacket + sizeof(CmdPacket.Header.PktCount);
	memcpy(&myBuffer[4], ptr, sizeof(char));
	memcpy(&myBuffer[5], &len, sizeof(len));
	memcpy(&myBuffer[6], bodyPtr, sizeBody);
	
	for (int i = 0; i < bufferSize; i++) {
		for (int j = 0; j < 8; j++) {
			count += (myBuffer[i] >> j) & 1;
		}
	}

	CmdPacket.CRC = count;
}

// /a function that takes a pointer to a RAW data buffer, the size of the packet in bytes located in the buffer, 
//and calculates the CRC. If the calculated CRC matches the CRC of the packet in the buffer the function returns TRUE, 
//otherwise FALSE.
bool PktDef::CheckCRC(char * rawBuffer, int numBytes) {
	int testCrc = numBytes - sizeof(CmdPacket.CRC);
	bool match = false;
	int count = 0;
	for (int i = 0; i < testCrc; i++) {
		for (int j = 0; j < 8; j++) {
			count += (rawBuffer[i] >> j) & 1;
		}
	}
	match = count == rawBuffer[testCrc] ? true : false;
	return match;
}
