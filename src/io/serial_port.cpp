/*
 * serial_port.cpp
 *
 *  Created on: Nov 2, 2014
 *      Author: rob
 */

#include "serial_port.h"

using namespace std;

const std::string SerialPortConfig::defaultSerialPort_("/dev/ttyS0");
const std::string SerialPortConfig::defaultBaudRate_("9600");
const std::string SerialPortConfig::defaultParityBits_("NONE");
const std::string SerialPortConfig::defaultDataBits_("8");
const std::string SerialPortConfig::defaultStopBits_("1");

///SerialPortConfig
SerialPortConfig::SerialPortConfig() {
	comPort_ = defaultSerialPort_;
	baudRate_ = defaultBaudRate_;
	parityBits_ = defaultParityBits_;
	dataBits_ = defaultDataBits_;
	stopBits_ = defaultStopBits_;
	streaming_ = false;
	verbose_ = false;
}
SerialPortConfig::SerialPortConfig(string port, string baud, string parity,
		string data, string stop, bool stream, bool verbose) {
	comPort_ = port;
	baudRate_ = baud;
	parityBits_ = parity;
	dataBits_ = data;
	stopBits_ = stop;
	streaming_ = stream;
	verbose_ = verbose;
}

SerialPort::SerialPort() {
	config_ = SerialPortConfig();
}

SerialPort::SerialPort(const SerialPortConfig& config) {
	config_ = config;
}

SerialPort::~SerialPort() {

}

int SerialPort::readWithTimeOut(char* buf, int size) {
	return readWithTimeOut(buf, size, defaultReadTimeoutMs);
}

