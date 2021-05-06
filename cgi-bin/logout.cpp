#define SQL_PASSWORD "root"

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <vector>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

#include "cookie.h"

using namespace sql;
using namespace std;
using namespace cgicc;

void redirectToLogin();

int main()
{
	// create cgicc instance for cookie
	Cgicc cgi;
	const_cookie_iterator cci;

	// create a mysql connection
	sql::Driver *driver;
	sql::Connection *con;
	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "root", SQL_PASSWORD);
	con->setSchema("HTML_DB");

	// get the cookie value
	char* cookie;
	const CgiEnvironment& env = cgi.getEnvironment();
	cci = env.getCookieList().begin();
	cookie = strdup((cci->getValue()).c_str());


	// delete the session
	sql::PreparedStatement *pstmt;

	string sql = "delete from session where cookie=?";
	pstmt = con->prepareStatement(sql);
	pstmt->setString(1, cookie);
	pstmt->execute();

	delete pstmt;
	delete con;

	// redirect to login page
	redirectToLogin();

	return 0;
}


void redirectToLogin()
{
	cout << "Content-type:text/html\r\n\r\n";
	cout << "<!doctype html>\n";
	cout << "<html lang=\"en\">\n";
	cout << "<body>\n";
	cout << "<script> window.location.href = \"/login.html\"; </script>\n";
	cout << "</body>\n";
	cout << "</html>\n";

	return;
}
