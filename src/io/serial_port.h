/*
 * serial_port.h
 *
 *  Created on: Nov 2, 2014
 *      Author: rob
 */

#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

#include <string>

class SerialPortConfig {
public:
	///Creates a serial port configuration with all default values
	SerialPortConfig();
	///Creates a serial port configuration with the values specified
	SerialPortConfig(std::string port, std::string baud, std::string parity,
			std::string data, std::string stop, bool stream, bool verbose);
	///Default dtor, does nothing
	~SerialPortConfig() {
	}
	///port name
	std::string comPort_;
	///baud rate {"1200","2400","9600","14400","28800","56600","115200"}
	std::string baudRate_;
	///parity bits {"ODD","NONE","EVEN", "MARK", "SPACE"}
	std::string parityBits_;
	///data bits {"5","6","7","8"}
	std::string dataBits_;
	///stop bits {"1","1.5","2"}
	std::string stopBits_;
	///a streaming port continuously receives data.  a polled port is non-streaming
	bool streaming_;
	///whether we print to std out during reads and writes
	bool verbose_;

	static const std::string defaultSerialPort_;
	static const std::string defaultBaudRate_;
	static const std::string defaultParityBits_;
	static const std::string defaultDataBits_;
	static const std::string defaultStopBits_;
};

class SerialPort {
public:
	///Generates a serial port object, with streaming and verbose set to false and a default configuration.
	SerialPort();
	///Generates a serial port object, with specified configuration.
	SerialPort(const SerialPortConfig& config);
	///Default dtor, closes the port
	virtual ~SerialPort();
	///sets the serial port's configuration object
	void setConfig(SerialPortConfig& config) {
		config_ = config;
	}
	///@return a const reference to the serial port's configuration object
	const SerialPortConfig& getConfig() {
		return config_;
	}
	///method for changing the baud rate
	void setBaudRate(std::string rate) {
		config_.baudRate_ = rate;
	}
	///method for setting the communications port used by the serial port
	void setCommPort(std::string port) {
		config_.comPort_ = port;
	}
	///method for changing the baud rate
	void setStreaming(bool streaming) {
		config_.streaming_ = streaming;
	}
	///method for setting the communications port used by the serial port
	void setVerbose(bool verbose) {
		config_.verbose_ = verbose;
	}
	///Verbose means a port should print information during send or receive
	///@return true if the port is verbose
	bool isVerbose() {
		return config_.verbose_;
	}
	///Streaming means a port produces data without polling.  Streaming ports are asynchronous.
	///@return true if the port is streaming
	bool isStreaming() {
		return config_.streaming_;
	}
	///@return true if the port is open
	virtual bool isOpen() = 0;
	///attempts to open the serial port.  all member variables must be preset.
	///@return false if failure occurs during opening
	///@throw IOException if the failure cannot be contained
	virtual bool open() = 0;
	///attempts to close the serial port
	///@return false if failure occurs during closing
	///@throw IOException if the failure cannot be contained
	virtual bool close() = 0;
	///blocking call.  will receive data into the pre-allocated buf of length size
	///@return the number of bytes actually read from the serial port
	virtual int read(char* buf, int size) = 0;
	///blocking call.  will transmit a buffer of data that has length of size
	///@return the number of bytes actually sent
	virtual int write(const char* data, int size) = 0;
	///blocking call.  will read until the termination character specified is seen in the stream or until size
	///characters are read, whichever happens first.
	///the caller is responsible for guaranteeing the size of the buffer
	///@return true if the termination character is found in the stream.  the buffer will be populated
	///with characters up to size
	virtual bool readUntil(char* buf, int size, const std::string &term, int timeoutMs) = 0;
	///blocking call.  will read up to size characters from the port or until the timeout has elapsed
	///@param buf the pre-allocated buffer to read data into
	///@param size must match the size of the pre-allocated buffer
	///@param timeout the timeout in milliseconds to wait to return from this call
	///@return the number of bytes actually read
	virtual int readWithTimeOut(char* buf, int size, int timeout) = 0;
	///read with defaultReadTimeoutMs
	int readWithTimeOut(char* buf, int size);
	static const int defaultReadTimeoutMs = 1000;

protected:
	///the configuration for the port
	SerialPortConfig config_;

private:
	///non-construction copyable
	SerialPort(const SerialPort& port);
	///non-copyable
	SerialPort& operator=(const SerialPort& port);

};

#endif /* SERIAL_PORT_H_ */
