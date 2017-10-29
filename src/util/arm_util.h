/*
 * arm_util.h
 *
 *  Created on: Nov 2, 2014
 *      Author: rob
 */

#ifndef ARM_UTIL_H_
#define ARM_UTIL_H_

#include <string>
#include <time.h>

///for formatting strings
std::string format(const char *fmtStr, ...);

class ARMUtil {

public:
	//static class
	ARMUtil(){}
	virtual ~ARMUtil(){}
	///@return the singleton instance of this class
	static ARMUtil& instance();
	///pause the executing thread for sleep milliseconds
	static void sleep(long sleep);
	///@return a double representing time since Jan 1, 1970
	static double timeNow();
    ///@return a tm struct (see time.h) representing current time
    static struct tm getCurrentTimeStruct();
    ///@return a tm struct (see time.h) representing given time
    static struct tm getTimeStruct(double time);
    ///@return a string representation of the current UTC time
    static std::string utcTimeString();
	///@return a string representing the int value passed in
	static std::string itos(int i);
	///@return a string representing the long value passed in
	static std::string ltos(unsigned long l);
	///@return a string of the date at the current time
	static std::string date();

    ///trace (to stdout or stderr) the given string
    static void trace(const char* buf);
    ///trace (to stdout or stderr) the given string
    static void trace(const std::string &str);

private:

    // No copy constructor
    ARMUtil(const ARMUtil &rhs){}

};

#endif /* ARM_UTIL_H_ */
