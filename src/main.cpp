//============================================================================
// Name        : main.cpp
// Author      : rdamus
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++
//============================================================================

#include <iostream>
using namespace std;

//
// Print a greeting message on standard output and exit.
//
// On embedded platforms this might require semi-hosting or similar.
//
// For example, for toolchains derived from GNU Tools for Embedded,
// to enable semi-hosting, the following was added to the linker:
//
// -Wl,--start-group -lgcc -lc -lc -lm -Wl,--end-group
//
// Adjust it for other toolchains.
//

#include "util/arm_util.h"
#include "io/linux_serial_port.h"
int main() {
	cout << "ARMUTilTests" << endl;

	ARMUtil::trace(format("current date: %s\n", ARMUtil::date().c_str()));
	ARMUtil::trace(format("timeNow(): %9.3f\n", ARMUtil::timeNow()));
	ARMUtil::trace(format("utcTime(): %s\n", ARMUtil::utcTimeString().c_str()));

	string comPort = "/dev/ttyO2";
	ARMUtil::trace(format("about to open the serial port, %s\n", comPort.c_str()));
	SerialPortConfig config;
	config.baudRate_ = "115200";
	config.comPort_ = comPort;
	LinuxSerialPort port(config);
	if( !port.open() ){
		ARMUtil::trace("failed to open port!\n");
		return -1;
	}else{
		ARMUtil::trace("opened port!\n");
		port.close();
	}


	return 0;
}
