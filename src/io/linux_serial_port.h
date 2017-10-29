/*
 * serial_port_interface.h
 *
 *  Created on: Nov 2, 2014
 *      Author: rob
 */

#ifndef SERIAL_PORT_INTERFACE_H_
#define SERIAL_PORT_INTERFACE_H_

#include "serial_port.h"
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

class SerialPortConfig;

class LinuxSerialPort : public SerialPort{
public:
	LinuxSerialPort();
	LinuxSerialPort(const SerialPortConfig& config);
	virtual ~LinuxSerialPort();
	///@return true if serial port is open, false otherwise
	virtual bool isOpen();
	///receive data into the pre-allocated buf of length size
	///@return the number of bytes actually read from the serial port
	virtual int read(char* buf, int size);
	///blocking call.  will read until the termination character specified is seen in the stream or until size
	///characters are read, whichever happens first.
	///the caller is responsible for guaranteeing the size of the buffer
	///@return true if the termination character is found in the stream.  the buffer will be populated
	///with characters up to size
	virtual bool readUntil(char* buf, int size, const std::string &term, int timeoutMs);
	///blocking call.  will read up to size characters from the port or until the timeout has elapsed
	///@param buf the pre-allocated buffer to read data into
	///@param size must match the size of the pre-allocated buffer
	///@param timeout the timeout in milliseconds to wait to return from this call
	///@return the number of bytes actually read
	virtual int readWithTimeOut(char* buf, int size, int timeout);
	///transmit a buffer of data that has length of size
	///@return the number of bytes actually sent
	virtual int write(const char* data, int size);
	///close this serial port
	virtual bool close();
	///connect to the serial port
	virtual bool open();
protected:
	virtual bool initPort();
	int parseBaudRate();
	int parseDataBits();
	int parseStopBits();
	int parseParityBits();
	int portFD_;
	struct termios oldPortOptions_;
	struct termios portOptions_;
};

#endif /* SERIAL_PORT_INTERFACE_H_ */
