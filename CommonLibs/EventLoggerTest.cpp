/*
 * EventLoggerTest.cpp
 *
 *  Created on: 28 May 2012
 *      Author: Keith Nolan
 */


#include "EventLogger.h"

using namespace std;

#define TEST_IMSI 		"12345678901234567"
#define TEST_NUMBER 	"2001"
#define TEST_EVENT 		"This is a test event"
#define TEST_MSG		"This is a test message"

int main(int argc, char * argv[] )
{
	cout << "Testing database connectivity" << endl;
	EventLogger *logger = EventLogger::getInstance();

	if (logger->insertIMSIAndNumber(TEST_IMSI, TEST_NUMBER)){
		cout << "IMSI and number insertion test succeeded" << endl;
	}
	else{
		cout << "IMSI and number insertion test FAILED" << endl;
	}

	cout << "IMSI id is " << logger->findIMSIId(TEST_IMSI) << endl;

	if (logger->insertEvent(TEST_IMSI,TEST_EVENT)){
		cout << "Event insertion test succeeded" << endl;
	}
	else{
		cout << "Event insertion test FAILED" << endl;
	}

	if (logger->insertSMS(TEST_IMSI,TEST_MSG,TEST_NUMBER,"")){
		cout << "SMS insertion test succeeded" << endl;
	}
	else{
		cout << "SMS insertion test FAILED" << endl;
	}

	if (logger->deleteIMSI(TEST_IMSI)){
		cout << "IMSI deletion test succeeded" << endl;
	}
	else{
		cout << "IMSI deletion test FAILED" << endl;
	}
}



