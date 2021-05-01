#include <stdlib.h>
#include <iostream>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;

int main()
{
try{
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;

	driver = get_driver_instance();
	con = driver->connect("localhost","root","root");
	stmt = con->createStatement();
	stmt -> execute("Create Database test");
	cout << "Create Table test";
	delete stmt;
	delete con;
	

} catch(sql::SQLException &e) {
	cout << "# ERR : SQLException in " << __FILE__;
	cout << "(" <<  __FUNCTION__ << ") on line >>"<<__LINE__<<endl;
	cout <<" (MySQL error code : " <<e.getErrorCode();
	cout <<", SQLState: " << e.getSQLState() << ")" <<endl;

}

cout <<endl;


return EXIT_SUCCESS;

}
