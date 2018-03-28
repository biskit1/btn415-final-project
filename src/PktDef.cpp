#include "PktDef.h"

void PktDef::SetPktCount(int count)
{
	this->CmdPacket.Header.PktCount = count;
}
