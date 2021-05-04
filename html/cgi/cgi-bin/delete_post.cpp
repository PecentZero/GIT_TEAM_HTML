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
void delete_post(string session_cookie,string f_post_id);
bool Check_auth(string session_value,string &username);
bool Check_post_auth(string post_id,string session_username);

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
	if(get_cookie_value(formData.getEnvironment(),session_name,session_value)
	&&   Check_Element(f_post_id,"post_id") && Check_auth(session_value,username) && Check_post_auth(**f_post_id,username))// exist session cookie , post id
	{

		delete_post(session_cookie,**f_post_id);
	}

	cout << "<script> alert(\""<<alert_msg<<"\");\n" <<endl;
	cout <<" history.back(); </script>\n";


}

	cout << "<br>\n";
	cout <<"</body>\n";
	cout <<"</html>\n";


return 0;
}

bool Check_Element(form_iterator &f,string msg) // print parameter's value
{ //also needed check space or valid value
	if(!f->isEmpty() && f != (*formData).end()) return true;
	else {
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


void delete_post(string session_cookie,string post_id)
{//return img_content from post_content

	string session_username, post_username,content_img;
	string globalpath= "../../";
	string alert_msg;

	sql::Driver *driver;
	sql::Connection *con;
	driver = get_driver_instance();
	con = driver->connect("localhost","root","root");
	con->setSchema("HTML_DB");
	sql::ResultSet *res_session;
	sql::ResultSet *res_post_content;
	sql::PreparedStatement *pstmt;


//session
string sql ="SELECT username from session where cookie = ?";
pstmt= con->prepareStatement(sql);
pstmt->setString(1,session_cookie);
res_session = pstmt->executeQuery(); // store result

if(res_session->next()) // session exist
{

	session_username = res_session->getString("username");
	 delete res_session;

	 string sql ="SELECT author_id,content_img from post_content where post_id = ?";
	 pstmt= con->prepareStatement(sql);
	 pstmt->setString(1,post_id);
   res_post_content = pstmt->executeQuery();

			 if(res_post_content->next()) // post exist
			 {

			      post_username = res_post_content->getString("author_id");
						content_img = res_post_content->getString("content_img");

						delete res_post_content;
						delete pstmt;

								if(session_username == post_username) //authentication
										{
											remove((globalpath+content_img).c_str()); //delete img file

											string sql ="DELETE from post_content where post_id = ?";//delete record
											pstmt= con->prepareStatement(sql);
											pstmt->setString(1,post_id);
										  pstmt->executeUpdate();
											delete pstmt;
											alert_msg = "Success in Deleting Post";

										}

									else alert_msg = "Permission Error"; 	//cout << "don't have permission"
			 }
			else alert_msg = "No matching post";//cout << "no matching post"
}

else alert_msg = "no matching session";//cout << "no matching session";

delete con;
cout << "<script> alert(\""<<alert_msg<<"\");" <<endl;
cout <<" history.back(); </script>";

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

void delete_post(string session_cookie,string post_id)
{//return img_content from post_content

	string session_username, post_username,content_img;
	string globalpath= "../../";
	string alert_msg;

	sql::Driver *driver;
	sql::Connection *con;
	driver = get_driver_instance();
	con = driver->connect("localhost","root","root");
	con->setSchema("HTML_DB");
	sql::ResultSet *res;
	sql::PreparedStatement *pstmt;



	 string sql ="SELECT content_img from post_content where post_id = ?";
	 pstmt= con->prepareStatement(sql);
	 pstmt->setString(1,post_id);
   res_post_content = pstmt->executeQuery();

			 if(res_post_content->next()) // post exist
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

				return 0;
}
