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

void redirectToMainWithCookie(char* cookie);
void redirectToLoginWithAlert();

void exploit()
{
	cout << "[TEAM HTML] Dummy Function for PoC\n";
}

int main()
{
	// create cgicc instance for cookie
	Cgicc cgi;

	// create a mysql connection
	sql::Driver *driver;
	sql::Connection *con;
	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "root", SQL_PASSWORD);
	con->setSchema("HTML_DB");


	// if already logged in

	// get the POST parameter for the userID
	char* userID;
	form_iterator fi = cgi.getElement("inputUsername");
	if ( !fi->isEmpty() && fi != (*cgi).end()){
		userID = strdup((**fi).c_str());
	} else {
		// nothing specified for the city name
		userID = "";
	}
	
	// check the length of userID
	if (strlen(userID) > 50){
		redirectToLoginWithAlert();	
		return 0;
	}

	// get the POST parameter for the password
	char* password;
	fi = cgi.getElement("inputPassword");
	if ( !fi->isEmpty() && fi != (*cgi).end()){
		password = strdup((**fi).c_str());
	} else {
		// nothing specified for the post page
		password = "";
	}
	
	// check the length of password
	if (strlen(password) > 50){
		redirectToLoginWithAlert();
		return 0;
	}

	sql::ResultSet *res;
	sql::PreparedStatement *pstmt_login;
	string sql = "select * from user_auth where username = ? and password = sha2( ? , 512)";
	pstmt_login = con->prepareStatement(sql);
	pstmt_login->setString(1, userID);
	pstmt_login->setString(2, password);
	
	res = pstmt_login->executeQuery();
	if (res->next())
	{
		sql::PreparedStatement *pstmt_del;
		string sql_del = "delete from session where username = ?";
		pstmt_del = con->prepareStatement(sql_del);
		pstmt_del->setString(1, userID);
		pstmt_del->execute();
		char* newCookie = generate_cookie(20, con);
		create_session(newCookie, userID, con);
		delete res;
		delete pstmt_login;
		delete pstmt_del;
		delete con;
		redirectToMainWithCookie(newCookie);
		return 0;
	}

	delete res;
	delete pstmt_login;
	delete con;
	redirectToLoginWithAlert();

	return 0;
}

void redirectToMainWithCookie(char* newCookie)
{
	cout << "Set-Cookie:session_id = " << newCookie << ";\r\n";
	cout << "Content-type:text/html\r\n\r\n";
	cout << "<!doctype html>\n";
	cout << "<html lang=\"en\">\n";
	cout << "<body>\n";
	cout << "<script> window.location.href = \"/cgi-bin/index.cgi\"; </script>\n";
	cout << "</body>\n";
	cout << "</html>\n";

	return;
}

void redirectToLoginWithAlert()
{
	cout << "Content-type:text/html\r\n\r\n";
	cout << "<!doctype html>\n";
	cout << "<html lang=\"en\">\n";
	cout << "<body>\n";
	cout << "<script> window.location.href = \"/login_fail.html\"; </script>\n";
	cout << "</body>\n";
	cout << "</html>\n";

	return;
}
