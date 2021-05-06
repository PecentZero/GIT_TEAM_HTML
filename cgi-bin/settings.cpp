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
void redirectToMain();

int main()
{
	// create cgicc instance for cookie
	Cgicc cgi;
	const_cookie_iterator cci;
	form_iterator fi;

	// get the POST parameter full-name
	char* fullName;
	fi = cgi.getElement("fullName");
	if ( !fi->isEmpty() && fi != (*cgi).end()){
		fullName = strdup((**fi).c_str());
	} else {
		// nothing specified for the city name
		fullName = "";
	}

	// get the POST parameter email
	char* email;
	fi = cgi.getElement("email");
	if ( !fi->isEmpty() && fi != (*cgi).end()){
		email = strdup((**fi).c_str());
	} else {
		// nothing specified for the city name
		email = "";
	}

	// get the POST parameter country
	char* country;
	fi = cgi.getElement("country");
	if ( !fi->isEmpty() && fi != (*cgi).end()){
		country = strdup((**fi).c_str());
	} else {
		// nothing specified for the city name
		country = "";
	}

	// get the POST parameter university
	char university[30];
	fi = cgi.getElement("university");
	if ( !fi->isEmpty() && fi != (*cgi).end()){
		strcpy(university,strdup((**fi).c_str()));
	} else {
		// nothing specified for the city name
		university = "";
	}

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

	// check the session
	char* username = session_check(cookie, con);
	if (username == NULL) {
		redirectToLogin();
		return 0;
	}

	// update the DB
	sql::PreparedStatement *pstmt;

	string sql = "update user_profile set fullname=?, university=?, email=?, nationality=? where username = ?";
	pstmt = con->prepareStatement(sql);
	pstmt->setString(1, fullName);
	pstmt->setString(2, university);
	pstmt->setString(3, email);
	pstmt->setString(4, country);
	pstmt->setString(5, username);
	pstmt->execute();

	delete pstmt;
	delete con;

	// redirect to login page
	redirectToMain();

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

void redirectToMain()
{
	cout << "Content-type:text/html\r\n\r\n";
	cout << "<!doctype html>\n";
	cout << "<html lang=\"en\">\n";
	cout << "<body>\n";
	cout << "<script> window.location.href = \"/\"; </script>\n";
	cout << "</body>\n";
	cout << "</html>\n";

	return;
}
