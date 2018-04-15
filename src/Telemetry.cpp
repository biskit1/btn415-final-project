#include "Telemetry.h"

Telemetry::Telemetry(char * body, int packetNum, std::string hex) {

	memcpy(&sonarData, &body[0], sizeof(sonarData));
	memcpy(&armData, &body[2], sizeof(armData));
	char *ptr = &body[4];
	driveFlag = *ptr & 1;
	armUp = (*ptr >> 1) & 1;
	armDown = (*ptr >> 2) & 1;
	clawOpen = (*ptr >> 3) & 1;
	clawClosed = (*ptr >> 4) & 1;
	numPacket = packetNum;
	hexString = hex;
}

std::string Telemetry::toString(displayType type) const
{
	std::string msg;
	std::string sep;

	if (type == SINGLE) {
		sep = " ";
	}
	else if (type == MULTI) {
		sep = "\n";
	}
	msg += "Packet: " + std::to_string(numPacket) + sep;
	msg += "Packet in Hex: " + hexString + sep;
	msg += "Sonar Reading: " + std::to_string(sonarData) + sep;
	msg += "Arm Reading: " + std::to_string(armData) + sep;
	if (driveFlag) {
		msg += "Drive flag: DRIVING" + sep;
	}
	else {
		msg += "Drive flag: STOPPED" + sep;
	}
	if (armUp) {
		msg += "Arm is Up, ";
	}
	else if (armDown) {
		msg += "Arm is Down, ";
	}
	if (clawOpen) {
		msg += "Claw is Open" + sep;
	}
	else if (clawClosed) {
		msg += "Claw is Closed" + sep;
	}
	msg += sep;
	return msg;
}


std::ostream& operator<<(std::ostream& os, const Telemetry& tele) {
	os << tele.toString(MULTI);
	return os;
}