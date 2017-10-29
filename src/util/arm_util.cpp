/*
 * arm_util.cpp
 *
 *  Created on: Nov 2, 2014
 *      Author: rob
 */

#include "arm_util.h"
#include <unistd.h>
#include <sys/times.h>
#include <termios.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <string.h>

using namespace std;

void ARMUtil::sleep(long sleep) {
	timespec TimeSpec;
	TimeSpec.tv_sec = sleep / 1000;
	TimeSpec.tv_nsec = (sleep % 1000) * 1000000;

	nanosleep(&TimeSpec, NULL);
}

double ARMUtil::timeNow() {
	double dfT = 0.0;
	struct timeb timebuffer;
	ftime(&timebuffer);
	dfT = timebuffer.time + ((double) timebuffer.millitm) / 1000;
	return dfT;
}

struct tm ARMUtil::getTimeStruct(double time) {
	struct tm timeStruct;
	time_t rawtime = static_cast<time_t>(time);
	gmtime_r(&rawtime, &timeStruct);
	return timeStruct;
}

std::string ARMUtil::utcTimeString() {
	struct tm timeStruct = getTimeStruct(timeNow());
	std::string timeStr = format("%d-%02d-%02dT%02d:%02d:%02dZ",
			1900 + timeStruct.tm_year, timeStruct.tm_mon + 1,
			timeStruct.tm_mday, timeStruct.tm_hour, timeStruct.tm_min,
			timeStruct.tm_sec);
	return timeStr;
}

std::string ARMUtil::date() {
	struct timeb timebuffer;
	ftime(&timebuffer);

	char *timeline = ctime(&(timebuffer.time));
	char sResult[100];
	sprintf(sResult, "%s", timeline);

	std::string sAnswer = sResult;

	return sAnswer;
}

///trace (to stdout or stderr) the given string
void ARMUtil::trace(const char *buf) {
	// arh changed this because if you wanted to add a percent character in the string, it would first
	// be processed by the _vsnprintf above, then placed in 'buf'.
	// Problem is that fprintf finds the '%' in buf and expects us to provide more arguments!
	//fprintf(stderr,buf);
	fputs(buf, stderr);
}

void ARMUtil::trace(const std::string &str) {
	trace(str.c_str());
}

#define MAX_FORMAT_STR 1024
std::string format(const char *fmtStr, ...)
{
    if( strlen(fmtStr) < MAX_FORMAT_STR ) {
        //double the size for format length!
        char buf[MAX_FORMAT_STR*2];
        va_list arg_ptr;
        va_start(arg_ptr, fmtStr);
        vsprintf(buf, fmtStr, arg_ptr);
        va_end(arg_ptr);
        return std::string(buf);
    } else {
        return "STRING TOO LONG TO FORMAT";
    }
}

///trace (to stdout or stderr) the given printf-style var-args parameters
void trace(const char *FmtStr, ...) {
	const unsigned int MAX_TRACE_STR = 2048;

	if (strlen(FmtStr) < MAX_TRACE_STR) {
		//double the size for format length!
		char buf[MAX_TRACE_STR * 2];

		va_list arg_ptr;

		va_start(arg_ptr, FmtStr);

		int n = vsnprintf(buf, sizeof(buf), FmtStr, arg_ptr);

		if (n == sizeof(buf)) {
			trace("WARNING trace() TRUNCATED TO %d CHARS", sizeof(buf));
		}

		va_end(arg_ptr);

		ARMUtil::trace(buf);
	}
}

void trace(const std::string &str) {
	ARMUtil::trace(str.c_str());
}
