#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <cgicc/CgiDefs.h>  //for cgic programming
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;
using namespace cgicc;

Cgicc formData;

bool Check_Element(form_iterator &f,string msg);
bool Check_ElementN(form_iterator &f);
bool get_cookie_value(const CgiEnvironment &env,string cookie_name, string &wanted_value);
void delete_adv(string adv_id);
bool Check_auth(string session_value,string &username);
bool Check_adv_auth(string adv_id,string session_username);
void delete_file_DB(string adv_id);

string alert_msg = "Success";
int main() {
	string adv_id,session_cookie;
	string session_name,session_value,username;
	session_name="session_id";
  cout << "Content-type:text/html\r\n\r\n";
	cout <<"<html>\n";
	cout <<"<head>\n";
	cout<<"<title>delete_adv</title>\n";
	cout <<"</head>\n";
	cout << "<body>\n";


	form_iterator f_adv_id = formData.getElement("adv_id"); //get adv_id element
	form_iterator f_type = formData.getElement("type"); // for edit

	if(get_cookie_value(formData.getEnvironment(),session_name,session_value)
	&&Check_Element(f_adv_id,"adv_id") && Check_auth(session_value,username) && Check_adv_auth((**f_adv_id),username))// exist session cookie , adv id
	{
		if(Check_ElementN(f_type))
		{
			if(**f_type == string("delfile"))
			delete_file_DB(**f_adv_id);
			else alert_msg ="type parameter error";
		}
		else
		delete_adv(**f_adv_id);
	}

	cout << "<script> alert(\""<<alert_msg<<"\");\n" <<endl;
	if(alert_msg == "Success")
	cout <<"location.href = \"/\"; </script>\n";
	else
	cout <<" history.back(); </script>\n";

	cout << "<br>\n";
	cout <<"</body>\n";
	cout <<"</html>\n";


return 0;
}

bool Check_Element(form_iterator &f,string msg) // print parameter's value
{ //also needed check space or valid value
	if(!f->isEmpty() && f != (*formData).end()) return true;
	else {
		if(!msg.empty())
		alert_msg = "missing " + msg;
		return false;
	}
}

bool Check_ElementN(form_iterator &f) // print parameter's value
{ //also needed check space or valid value
	if(!f->isEmpty() && f != (*formData).end()) return true;
	else {
		return false;
	}
}

bool get_cookie_value(const CgiEnvironment &env,string cookie_name, string &wanted_value)
{
     const_cookie_iterator iter;
     for(iter = env.getCookieList().begin(); iter != env.getCookieList().end(); ++iter)
		 {
					if(iter->getName() == cookie_name){
					wanted_value = (iter->getValue());
					return true;
				}
			}
		alert_msg = "Login First!";
    return false;
}

bool Check_auth(string session_value,string &username)
{

		sql::Driver *driver;
		sql::Connection *con;
		sql::ResultSet *res;
		sql::PreparedStatement *pstmt;

		driver = get_driver_instance();
		con = driver->connect("localhost","root","root");
		con->setSchema("HTML_DB");

		//session
		string sql ="SELECT username from session where cookie = ?";
		pstmt= con->prepareStatement(sql);
		pstmt->setString(1,session_value);
		res = pstmt->executeQuery(); // store result
		delete pstmt;

		if(res->next()) // session exist
		{
			username = res->getString("username");
		 	delete res;
			return true;
		}
		delete con;
		alert_msg = "Try login again!";
		return false;
}

bool Check_adv_auth(string adv_id,string session_username)
{

		string adv_username;

		sql::Driver *driver;
		sql::Connection *con;
		sql::ResultSet *res;
		sql::PreparedStatement *pstmt;

		driver = get_driver_instance();
		con = driver->connect("localhost","root","root");
		con->setSchema("HTML_DB");

		string sql ="SELECT author_id from adv_content where adv_id = ?";
		pstmt= con->prepareStatement(sql);
		pstmt->setString(1,adv_id);
		res= pstmt->executeQuery();

				 if(res->next()) // adv exist
				 {

							adv_username = res->getString("author_id");

							delete res;
							delete pstmt;

									if(session_username == adv_username) //authentication
									{
										delete con;
									return true;
								}
						}
					alert_msg ="No matching adv";
			delete con;
			return false;
}


void delete_adv(string adv_id)
{//return img_content from adv_content

		string content_img;
		string globalpath= "../../";

		sql::Driver *driver;
		sql::Connection *con;
		sql::ResultSet *res;
		sql::PreparedStatement *pstmt;

		driver = get_driver_instance();
		con = driver->connect("localhost","root","root");
		con->setSchema("HTML_DB");

	 string sql ="SELECT content_img from adv_content where adv_id = ?";
	 pstmt= con->prepareStatement(sql);
	 pstmt->setString(1,adv_id);
	 res = pstmt->executeQuery();

			 if(res->next()) // adv exist
			 {
						content_img = res->getString("content_img");
						delete res;
						delete pstmt;
						if(!content_img.empty())
						remove((globalpath+content_img).c_str()); //delete img file

						string sql ="DELETE from adv_content where adv_id = ?";//delete record
						pstmt= con->prepareStatement(sql);
						pstmt->setString(1,adv_id);
					  pstmt->executeUpdate();
						delete pstmt;
				}

}


void delete_file_DB(string adv_id)
{
	string delete_img; // should delete
  string globalpath ="../../";
	sql::Driver *driver;
	sql::Connection *con;
	sql::PreparedStatement *pstmt;
	sql::ResultSet *res;
	driver = get_driver_instance();
	con = driver->connect("localhost","root","root");
	con->setSchema("HTML_DB");


	string sql ="select content_img from adv_content where adv_id = ?";
	pstmt = con->prepareStatement(sql);
	pstmt->setString(1,adv_id);
	res = pstmt->executeQuery();
	res->next();
	delete_img = res->getString("content_img");

	delete pstmt;
	delete res;

	if(!delete_img.empty()) // prior file exists so should delete
 {	remove((globalpath+delete_img).c_str()); //delete img file


	sql = "Update adv_content set content_img = NULL,time_written = NOW() where adv_id = ?";
	pstmt = con->prepareStatement(sql);
	pstmt->setString(1,adv_id);
	pstmt->executeUpdate();

	delete pstmt;
	alert_msg ="Success Delete file";
}


else{ //keep prior file state
alert_msg ="NO attached file";
}

	delete con;
}