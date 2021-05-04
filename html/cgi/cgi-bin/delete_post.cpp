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
bool get_cookie_value(const CgiEnvironment &env,string cookie_name, string &wanted_value);
void delete_post(string post_id);
bool Check_auth(string session_value,string &username);
bool Check_post_auth(string post_id,string session_username);
void delete_file_DB(string post_id);

string alert_msg = "Success Delete";
int main() {
	string post_id,session_cookie;
	string session_name,session_value,username;
	session_name="session_id";
  cout << "Content-type:text/html\r\n\r\n";
	cout <<"<html>\n";
	cout <<"<head>\n";
	cout<<"<title>delete_post</title>\n";
	cout <<"</head>\n";
	cout << "<body>\n";


	form_iterator f_post_id = formData.getElement("post_id"); //get post_id element
	form_iterator f_type = formData.getElement("type"); // for edit

	if(get_cookie_value(formData.getEnvironment(),session_name,session_value)
	&&Check_Element(f_post_id,"post_id") && Check_auth(session_value,username) && Check_post_auth(**f_post_id,username))// exist session cookie , post id
	{
		if(Check_Element(f_type,NULL))
		{
			if(**f_type == string("delfile"))
			delete_file_DB(**f_post_id);
			else alert_msg ="type parameter error";
		}
		else
		delete_post(**f_post_id);
	}

	cout << "<script> alert(\""<<alert_msg<<"\");\n" <<endl;
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

bool Check_post_auth(string post_id,string session_username)
{

		string post_username;

		sql::Driver *driver;
		sql::Connection *con;
		sql::ResultSet *res;
		sql::PreparedStatement *pstmt;

		driver = get_driver_instance();
		con = driver->connect("localhost","root","root");
		con->setSchema("HTML_DB");

		string sql ="SELECT author_id from post_content where post_id = ?";
		pstmt= con->prepareStatement(sql);
		pstmt->setString(1,post_id);
		res= pstmt->executeQuery();

				 if(res->next()) // post exist
				 {

							post_username = res->getString("author_id");

							delete res;
							delete pstmt;

									if(session_username == post_username) //authentication
									{
										delete con;
									return true;
								}
						}
					alert_msg ="No matching post";
			delete con;
			return false;
}


void delete_post(string post_id)
{//return img_content from post_content

		string content_img;
		string globalpath= "../../";

		sql::Driver *driver;
		sql::Connection *con;
		sql::ResultSet *res;
		sql::PreparedStatement *pstmt;

		driver = get_driver_instance();
		con = driver->connect("localhost","root","root");
		con->setSchema("HTML_DB");

	 string sql ="SELECT content_img from post_content where post_id = ?";
	 pstmt= con->prepareStatement(sql);
	 pstmt->setString(1,post_id);
	 res = pstmt->executeQuery();

			 if(res->next()) // post exist
			 {
						content_img = res->getString("content_img");
						delete res;
						delete pstmt;
						if(content_img != string("NULL"))
						remove((globalpath+content_img).c_str()); //delete img file

						string sql ="DELETE from post_content where post_id = ?";//delete record
						pstmt= con->prepareStatement(sql);
						pstmt->setString(1,post_id);
					  pstmt->executeUpdate();
						delete pstmt;
				}

}


void delete_file_DB(string post_id)
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


	string sql ="select content_img from post_content where post_id = ?";
	pstmt = con->prepareStatement(sql);
	pstmt->setString(1,post_id);
	res = pstmt->executeQuery();
	res->next();
	delete_img = res->getString("content_img");

	delete pstmt;
	delete res;

	if(!delete_img.empty()) // prior file exists so should delete
 {	remove((globalpath+delete_img).c_str()); //delete img file


	sql = "Update post_content set content_img = NULL,time_written = NOW() where post_id = ?";
	pstmt = con->prepareStatement(sql);
	pstmt->setString(1,post_id);
	pstmt->executeUpdate();

	delete pstmt;
	alert_msg ="Success Delete file";
}


else{ //keep prior file state
alert_msg ="NO attached file";
}

	delete con;
}
