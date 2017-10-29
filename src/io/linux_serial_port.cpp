/*
 * serial_port_interface.cpp
 *
 *  Created on: Nov 2, 2014
 *      Author: rob
 */

#include "linux_serial_port.h"
#include "arm_util.h"
#include <memory.h>

LinuxSerialPort::LinuxSerialPort() :
		SerialPort() {
	portFD_ = -1;
}

LinuxSerialPort::LinuxSerialPort(const SerialPortConfig& config) :
		SerialPort(config) {
	portFD_ = -1;
}

LinuxSerialPort::~LinuxSerialPort() {
	// TODO Auto-generated destructor stub
}

bool LinuxSerialPort::isOpen() {
	return portFD_ > 0;
}

int LinuxSerialPort::read(char* buf, int size) {
	if (!isOpen() || isStreaming())
		return -1;

	return ::read(portFD_, buf, size);
}

bool LinuxSerialPort::readUntil(char* buf, int size, const std::string& term,
		int timeout) {
	double startTime = ARMUtil::timeNow();
	int charsRead = 0;
	bool foundIt = false;
	while ((ARMUtil::timeNow() - startTime) < timeout) {
		int rx = read(buf + charsRead, 1);
		if (rx) {
			charsRead += rx;
			//check for the string
			for (int i = 0; i < charsRead; i++) {
				const char* str = term.c_str();
				if (buf[i] == *str){
					foundIt = true;
				}
			}
		} else {
			//received negative characters
			break;
		}
	}
	return foundIt ? : (charsRead == size);
}

int LinuxSerialPort::readWithTimeOut(char* buf, int size, int timeout) {
	double startTime = ARMUtil::timeNow();
	int charsRead = 0;
	bool foundIt = false;
	while ((ARMUtil::timeNow() - startTime) < timeout && (charsRead < size)) {
		int rx = read(buf + charsRead, 1);
		if (rx) {
			charsRead += rx;
		} else {
			break;
		}
	}
	return charsRead;
}

int LinuxSerialPort::write(const char* data, int size) {
	if (!isOpen())
		return -1;
	return ::write(portFD_, data, size);
}

bool LinuxSerialPort::close() {
	if (!isOpen())
		return true;
	::tcsetattr(portFD_, TCSANOW, &oldPortOptions_);

	bool res = ::close(portFD_) == 0;
	portFD_ = -1;

	return res;
}

bool LinuxSerialPort::open() {
	if (isOpen())
		return true;

	// open and configure the serial port
	portFD_ = ::open(config_.comPort_.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

	if (portFD_ < 0) {
		perror(config_.comPort_.c_str());
		return false;
	}

	return initPort();
}

bool LinuxSerialPort::initPort() {
	//save the current configuration
	tcgetattr(portFD_, &oldPortOptions_);

	//zero the buffers
	memset(&portOptions_, 0, sizeof(portOptions_));

	int baud = parseBaudRate();
	int data = parseDataBits();
	int stop = parseStopBits();
	int parity = parseParityBits();

	portOptions_.c_cflag = baud | data | stop | parity | CLOCAL | CREAD;
	portOptions_.c_iflag = IGNPAR;
	portOptions_.c_oflag = 0;

	/* set input mode (non-canonical, no echo,...) */
	portOptions_.c_lflag = 0;

	// inter-character timer unused
	portOptions_.c_cc[VTIME] = 0;
	// blocking read until 0 chars received, i.e. don't block
	portOptions_.c_cc[VMIN] = 0;

	//save the new settings
	tcflush(portFD_, TCIFLUSH);
	tcsetattr(portFD_, TCSANOW, &portOptions_);

	return portFD_ != 0;
}

int LinuxSerialPort::parseBaudRate() {
	int defaultBaudRate = B9600;

	if (config_.baudRate_ == "115200")
		return B115200;
	else if (config_.baudRate_ == "57600")
		return B57600;
	else if (config_.baudRate_ == "38400")
		return B38400;
	else if (config_.baudRate_ == "19200")
		return B19200;
	else if (config_.baudRate_ == "9600")
		return B9600;
	else if (config_.baudRate_ == "4800")
		return B4800;
	else if (config_.baudRate_ == "2400")
		return B2400;
	else if (config_.baudRate_ == "1200")
		return B1200;
	else if (config_.baudRate_ == "600")
		return B600;
	else if (config_.baudRate_ == "300")
		return B300;
	else
		return defaultBaudRate;
}

int LinuxSerialPort::parseDataBits() {
	int defaultDataBits = CS8;

	if (config_.dataBits_ == "5")
		return CS5;
	else if (config_.dataBits_ == "6")
		return CS6;
	else if (config_.dataBits_ == "7")
		return CS7;
	else if (config_.dataBits_ == "8")
		return CS8;
	else
		return defaultDataBits;
}

int LinuxSerialPort::parseParityBits() {
	int defaultParityBits = 0;

	if (config_.parityBits_ == "ODD")
		return PARODD | PARENB;
	else if (config_.parityBits_ == "EVEN")
		return PARENB;
	else if (config_.parityBits_ == "NONE")
		return 0;
	else
		return defaultParityBits;
}

int LinuxSerialPort::parseStopBits() {
	int defaultStopBits = 1;

	if (config_.parityBits_ == "2")
		return CSTOPB;
	else
		return defaultStopBits;
}
