#pragma once
#include <iostream>
#include <fstream>
#include <string>

enum displayType { SINGLE, MULTI };

class Telemetry {
	short int sonarData;
	short int armData;
	bool driveFlag;
	bool armUp;
	bool armDown;
	bool clawOpen;
	bool clawClosed;
	int numPacket;
	std::string hexString;

public:
	Telemetry(char *, int, std::string);
	std::string toString(displayType) const;

};
std::ostream& operator<<(std::ostream&, const Telemetry&);