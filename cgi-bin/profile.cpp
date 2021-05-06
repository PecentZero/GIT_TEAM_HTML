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

void makeProfilePage(char* fullname, char* university, char* email, char* nationality);
void profileFirstPart(char* fullname, char* university);
void profileSubfunction(char* title, char* content);
void redirectToMain();
void redirectToLogin();
void exploit();

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

	// check the session
	char* userID = session_check(cookie, con);

	if (!userID)
	{
		// if not logged in, redirect to the login page
		redirectToLogin();
		return 0;
	}

	// if already logged in
	// get the GET parameter for the city
	char user_id[30];
	form_iterator fi = cgi.getElement("user_id");
	if ( !fi->isEmpty() && fi != (*cgi).end()){
		strcpy(user_id, strdup((**fi).c_str()));
	} else {
		// nothing specified for the username
		// default to the user's profile
		strcpy(user_id, userID);
	}

	// get the data from the DB with key user_id as username
	sql::PreparedStatement *pstmt;
	sql::ResultSet *res;

	string sql = "select * from user_profile where username=?";
	pstmt = con->prepareStatement(sql);
	pstmt->setString(1, user_id);
	res = pstmt->executeQuery();

	if (res->next())
	{
		// proper userid
		char* fullname = strdup(res->getString("fullname").c_str());
		char* university = strdup(res->getString("university").c_str());
		char* email = strdup(res->getString("email").c_str());
		char* nationality = strdup(res->getString("nationality").c_str());
		delete res;
		delete pstmt;
		// create the page
		makeProfilePage(fullname, university, email, nationality);
	}
	else
	{
		// unknown userid
		delete res;
		delete pstmt;
		redirectToMain();
	}

	delete con;

	return 0;
}

void exploit()
{
	cout << "<p>[Team HTML] Dummy Function for PoC</p>\n";
}

void makeProfilePage(char* fullname, char* university, char* email, char* nationality)
{
	cout << "Content-type:text/html\r\n\r\n";
	profileFirstPart(fullname, university);
	profileSubfunction("Full Name", fullname);
	cout << "<hr>\n";
	profileSubfunction("Email", email);
	cout << "<hr>\n";
	profileSubfunction("Home Country", nationality);
	cout << "<hr>\n";
	profileSubfunction("University", university);
	cout << "</div>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "</div>\n";
	cout << "</body>\n";
	cout << "</html>\n";
}

void redirectToMain()
{
	cout << "Content-type:text/html\r\n\r\n";
	cout << "<!doctype html>\n";
	cout << "<html lang=\"en\">\n";
	cout << "<body>\n";
	cout << "<script> alert(\"The user's profile is not registered.\"); window.location.href = \"/\"; </script>\n";
	cout << "</body>\n";
	cout << "</html>\n";

	return;

}

void redirectToLogin()
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

void profileFirstPart(char* fullname, char* university)
{
	cout << "<!DOCTYPE html>\n";
	cout << "<html lang=\"en\">\n";
	cout << "<head>\n";
	cout << "<meta charset=\"utf-8\">\n";
	cout << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">\n";
	cout << "<meta name=\"description\" content=\"\">\n";
	cout << "<meta name=\"author\" content=\"\">\n";
	cout << "<title>Team HTML</title>\n";
	cout << "<link rel=\"stylesheet\" href=\"/css/bootstrap.css\">\n";
	cout << "<link rel=\"stylesheet\" href=\"/vendors/linericon/style.css\">\n";
	cout << "<link rel=\"stylesheet\" href=\"/css/font-awesome.min.css\">\n";
	cout << "<link rel=\"stylesheet\" href=\"/vendors/lightbox/simpleLightbox.css\">\n";
	cout << "<link rel=\"stylesheet\" href=\"/vendors/nice-select/css/nice-select.css\">\n";
	cout << "<!-- main css -->\n";
	cout << "<link rel=\"stylesheet\" href=\"/css/style.css\">\n";
	cout << "<link rel=\"stylesheet\" href=\"/css/responsive.css\">\n";
	cout << "<!-- Bootstrap core CSS -->\n";
	cout << "<link href=\"/vendors/bootstrap/css/bootstrap.min.css\" rel=\"stylesheet\">\n";
	cout << "<!-- Custom styles for this template -->\n";
	cout << "<link href=\"/css/simple-sidebar.css\" rel=\"stylesheet\">\n";
	cout << "<link href=\"/css/style_profile.css\" rel=\"stylesheet\">\n";
	cout << "</head>\n";
	cout << "<body>\n";
	cout << "<div class=\"container\">\n";
	cout << "<div class=\"main-body\">\n";
	cout << "<!-- Breadcrumb -->\n";
	cout << "<nav aria-label=\"breadcrumb\" class=\"main-breadcrumb\">\n";
	cout << "<ol class=\"breadcrumb\">\n";
	cout << "<li class=\"breadcrumb-item\"><a href=\"/index.html\">Home</a></li>\n";
	cout << "</ol>\n";
	cout << "</nav>\n";
	cout << "<!-- /Breadcrumb -->\n";
	cout << "<div class=\"row gutters-sm\">\n";
    cout << "<div class=\"col-md-4 mb-3\">\n";
    cout << "<div class=\"card\">\n";
    cout << "<div class=\"card-body\">\n";
    cout << "<div class=\"d-flex flex-column align-items-center text-center\">\n";
    cout << "<img src=\"https://bootdey.com/img/Content/avatar/avatar7.png\" alt=\"Admin\" class=\"rounded-circle\" width=\"150\">\n";
    cout << "<div class=\"mt-3\">\n";
    cout << "<h4>" << fullname << "</h4>\n";
    cout << "<p class=\"text-secondary mb-1\">" << university << "</p><br>\n";
    cout << "<a href=\"../settings.html\"><button class=\"btn btn-primary\">Edit my page</button></a>\n";
    cout << "</div>\n";
   	cout << "</div>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "<div class=\"col-md-8\">\n";
    cout << "<div class=\"card mb-3\">\n";
    cout << "<div class=\"card-body\">\n";
}

void profileSubfunction(char* title, char* content)
{
	cout << "<div class=\"row\">\n";
    cout << "<div class=\"col-sm-3\">\n";
    cout << "<h6 class=\"mb-0\">" << title << "</h6>\n";
    cout << "</div>\n";
    cout << "<div class=\"col-sm-9 text-secondary\">\n";
    cout << content;
    cout << "</div>\n";
    cout << "</div>\n";
}
