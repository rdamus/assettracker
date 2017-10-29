/*
 * ARMUtil_test.cpp
 *
 *  Created on: Nov 2, 2014
 *      Author: rob
 */

#include "arm_util.h"

void testDateTime(){

	ARMUtil::instance().trace(format("current date: %s", ARMUtil::instance().date().c_str() ) );
	ARMUtil::instance().trace(format("timeNow(): %9.3f", ARMUtil::instance().timeNow()) );
	ARMUtil::instance().trace(format("utcTime(): %s", ARMUtil::instance().utcTimeString().c_str() ) );
}
