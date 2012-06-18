/**
 * 	EventLogger.h
 *
 *  Created on: 28 May 2012
 *  Author: Keith Nolan
 */

#ifndef EVENTLOGGER_H_
#define EVENTLOGGER_H_

using namespace std;

#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#define DBHOST "tcp://127.0.0.1:3306"
#define USER "root"
#define PASSWORD ""
#define DATABASE "openbts"

class EventLogger {

	private:

	sql::mysql::MySQL_Driver *driver;
	sql::Connection	*con;
	sql::Statement	*stmt;
	sql::ResultSet *res;

	//Prepared statements
	sql::PreparedStatement *prep_stmt_imsi_insert;
	sql::PreparedStatement *prep_stmt_imsi_delete;
	sql::PreparedStatement *prep_stmt_event_insert;
	sql::PreparedStatement *prep_stmt_imsi_find;
	sql::PreparedStatement *prep_stmt_sms_insert;


	int updateCount;

	//Singleton class instance pointer
	static EventLogger *pEventLogger;

	private:

	//Ctor - singleton class
	EventLogger();


	public:

	//Dtor
	~EventLogger();

	//Inserts an IMSI into the database
	bool insertIMSIAndNumber(std::string IMSI, std::string number);

	//Deletes an IMSI from the database
	//This also removes all events and messages associated with the IMSI
	bool deleteIMSI(std::string IMSI);

	//Inserts an event message into the database. Each event message is attached to an IMSI
	bool insertEvent(std::string IMSI, std::string event);

	//Finds the record ID associated with an IMSI
	uint32_t findIMSIId(std::string IMSI);

	//Inserts a sent/received SMS into the database
	bool insertSMS(std::string IMSI, std::string SMS, std::string to, std::string from);

	//Overloaded method that does not include the "to" parameter
	bool insertSMS(std::string IMSI, std::string SMS, std::string from);

	//Returns a singleton class instance of the EventLogger
	static EventLogger* getInstance();

};


#endif /* EVENTLOGGER_H_ */
