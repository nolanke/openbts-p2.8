/**
 * 	EventLogger.cpp
 *
 *  Created on: 28 May 2012
 *  Author: Keith Nolan
 */

#include "EventLogger.h"

/**
	Constructor
	@param none
*/

EventLogger* EventLogger::pEventLogger = NULL;


EventLogger* EventLogger::getInstance()
{
	//lock_mutex(); //TODO thread safety
	if (!pEventLogger)
	{
		pEventLogger = new EventLogger();
	}
	//unlock_mutex(); //TODO thread safety
	return pEventLogger;
}


EventLogger::EventLogger() : updateCount(0)
{
	//Get a handle to the MySQL driver
	driver = sql::mysql::get_mysql_driver_instance();

	//Attempt to connect to the database
	con = driver -> connect(DBHOST,"root","");

	//Create a statement and select the database
	stmt = con -> createStatement();
	stmt -> execute("USE " DATABASE);

	//Create the prepared statements
	prep_stmt_imsi_insert = con -> prepareStatement ("INSERT INTO IMSIs(IMSI, number) VALUES(?, ?)");

	prep_stmt_imsi_delete = con -> prepareStatement ("DELETE FROM IMSIs WHERE imsi = ?");

	prep_stmt_event_insert = con -> prepareStatement ("INSERT INTO events(event, IMSIs_id) "
			"VALUES(?, (SELECT id FROM IMSIs WHERE IMSIs.IMSI=?))");

	prep_stmt_imsi_find = con -> prepareStatement ("SELECT id FROM IMSIs WHERE IMSI = ?");

	prep_stmt_sms_insert = con -> prepareStatement ("INSERT INTO SMSes (msg,sender,recipient,IMSIs_id) VALUES(?,?,?,(SELECT id FROM IMSIs WHERE IMSIs.IMSI=?))");

}

/**
	Destructor
	@param none
*/
EventLogger::~EventLogger()
{
	//delete res;
	delete stmt;
	delete con;
}

/**
	Inserts an IMSI into the MySQL event database
	@param IMSI a string representation of the IMSI
*/
bool EventLogger::insertIMSIAndNumber(std::string IMSI, std::string number)
{
	int updateCount = 0;

	prep_stmt_imsi_insert -> setString(1,IMSI);
	prep_stmt_imsi_insert -> setString(2,number);

	try{
		updateCount = prep_stmt_imsi_insert -> executeUpdate();
	}
	catch(sql::SQLException e){
		std::cout << "Exception caught - insertIMSIAndNumber(" << IMSI << "," << number << "): " << e.getSQLState() << ":" << e.getErrorCode() << std::endl;
		return false;
	}
	return updateCount;
}


/**
 * Attempts to find the record ID associated with an IMSI
 * @param IMSI a string representation of the IMSI
 */
uint32_t EventLogger::findIMSIId(std::string IMSI)
{
	uint32_t id = 0;
	prep_stmt_imsi_find -> setString(1,IMSI);

	res = NULL;
	try{
		res = prep_stmt_imsi_find->executeQuery();
		res->next();
		id = res->getInt("id");
	}
	catch (sql::SQLException e){
		std::cout << "Exception caught - findIMSIId(" << IMSI << "): " << e.getSQLState() << ":" << e.getErrorCode() << std::endl;
		return id;
	}
	return id;
}

/**
	Removes an IMSI from the MySQL event database.
	This also removes all events and messages associated with this IMSI
	@param IMSI a string representation of the IMSI
*/
bool EventLogger::deleteIMSI(std::string IMSI)
{
	int updateCount = 0;

	prep_stmt_imsi_delete -> setString(1,IMSI);

	try{
		updateCount = prep_stmt_imsi_delete -> executeUpdate();
	}
	catch (sql::SQLException e){
		std::cout << "Exception caught - deleteIMSI(" << IMSI << ")" << e.getSQLState() << ":" << e.getErrorCode() << std::endl;
		return false;
	}
	return updateCount;
}

/**
	Inserts an event into the MySQL event database
	@param IMSI		a string representation of the IMSI
	@param event 	the event string
*/
bool EventLogger::insertEvent(std::string IMSI, std::string event)
{
	int updateCount = 0;

	prep_stmt_event_insert -> setString(1,event);
	prep_stmt_event_insert -> setString(2,IMSI);

	try {
		updateCount = prep_stmt_event_insert -> executeUpdate();
	}
	catch(sql::SQLException e){
		//std::cout << "Exception caught - insertEvent (" << IMSI << "," << event << "): " << e.getSQLState() << ":" << e.getErrorCode() << std::endl;
		return false;
	}
	return updateCount;
}


bool EventLogger::insertSMS(std::string IMSI, std::string SMS, std::string to, std::string from)
{
	int updateCount = 0;
	prep_stmt_sms_insert -> setString(1,SMS);
	prep_stmt_sms_insert -> setString(2,from);
	prep_stmt_sms_insert -> setString(3,to);
	prep_stmt_sms_insert -> setString(4,IMSI);

	try {
		updateCount = prep_stmt_sms_insert -> executeUpdate();
	}
	catch(sql::SQLException e){
		return false;
	}
	return updateCount;
}

bool EventLogger::insertSMS(std::string IMSI, std::string SMS, std::string from)
{
	cout << "IMSI: " << IMSI << ", SMS: " << SMS << "from: " << from << endl;
	return insertSMS(IMSI,SMS,"",from);
}
