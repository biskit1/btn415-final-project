#pragma once

const int FORWARD = 1;
const int BACKWARD = 2;
const int RIGHT = 3;
const int LEFT = 4;
const int UP = 5;
const int DOWN = 6;
const int OPEN = 7;
const int CLOSE = 8;
const int HEADERSIZE = 6;

enum CmdType { DRIVE = 1, STATUS, SLEEP, ARM, CLAW, ACK, UNKNOWN };

struct MotorBody {
	char Direction;
	char Duration; 
};

struct ActuatorBody {
	char Action;
};

class PktDef {
	struct Header {
		unsigned int PktCount;
		unsigned char Drive : 1;
		unsigned char Status : 1;
		unsigned char Sleep : 1;
		unsigned char Arm : 1;
		unsigned char Claw : 1;
		unsigned char Ack : 1;
		unsigned char Padding : 2;
		unsigned char Length;
	};

	struct Packet {
		Header Header;
		char* Data;
		char CRC;
	};

	enum CmdFlag { ALL };

	char* RawBuffer;
	Packet CmdPacket;

	int countFlags();
	void clearFlag(const CmdFlag&);
	char* GetFlagData() const;
  
public:
	PktDef();
	PktDef(char*);
	char* GenPacket();
	void SetBodyData(char*, int);
	CmdType GetCmd();
	bool GetAck();
	int GetLength();
	char* GetBodyData();
	int GetPktCount();
	void SetPktCount(int);
	void SetCmd(const CmdType&);
	bool CheckCRC(char*, int);
	void CalcCRC();
};
