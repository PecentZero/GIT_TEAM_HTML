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

bool Check_Element(form_iterator &f);
bool get_cookie_value(const CgiEnvironment &env,string cookie_name, string &wanted_value);
void delete_post(sql::Connection *con ,string session_cookie,string f_post_id);
int main() {
	string post_id, session_value,session_cookie,img_content;
  cout << "Content-type:text/html\r\n\r\n";
	cout <<"<html>\n";
	cout <<"<head>\n";
	cout<<"<title>delete_post</title>\n";
	cout <<"</head>\n";
	cout << "<body>\n";


	form_iterator f_post_id = formData.getElement("post_id"); //get post_id element
  if(get_cookie_value(formData.getEnvironment(),string("session"),session_cookie)
&&   Check_Element(f_post_id))// exist session cookie , post id
	{

		sql::Driver *driver;
		sql::Connection *con;
		driver = get_driver_instance();
		con = driver->connect("localhost","root","root");
		con->setSchema("HTML_DB");
		delete_post(con ,session_cookie,**f_post_id);
	}

else{
	cout << "<script> alert(\"Missing post_id or session\");" <<endl;
	cout <<" history.back(); </script>";


}

	cout << "<br>\n";
	cout <<"</body>\n";
	cout <<"</html>\n";


return 0;
}

bool Check_Element(form_iterator &f) // print parameter's value
{ //also needed check space or valid value
	if(!f->isEmpty() && f != (*formData).end()) return true;
	else return false;
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

    return false;
}

void delete_post(sql::Connection *con ,string session_cookie,string post_id)
{//return img_content from post_content
	string session_username, post_username,img_content;
	string globalpath= "../../";
	string alert_msg;
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

	 string sql ="SELECT author_id,img_content from post_content where post_id = ?";
	 pstmt= con->prepareStatement(sql);
	 pstmt->setString(1,post_id);
   res_post_content = pstmt->executeQuery();

			 if(res_post_content->next()) // post exist
			 {

			      post_username = res_post_content->getString("author_id");
						img_content = res_post_content->getString("img_content");

						delete res_post_content;
						delete pstmt;

								if(session_username == post_username) //authentication
										{
											remove((globalpath+img_content).c_str()); //delete img file

											string sql ="DELETE * from post_content where post_id = ?";//delete record
											pstmt= con->prepareStatement(sql);
											pstmt->setString(1,post_id);
										  pstmt->executeUpdate();
											delete pstmt;
											alert_msg += "Success in Deleting Post";

										}

									else alert_msg += "Permission Error"; 	//cout << "don't have permission"
			 }
			else alert_msg += "No matching post";//cout << "no matching post"
}

else alert_msg += "no matching session |";//cout << "no matching session";


cout << "<script> alert("<<alert_msg<<");" <<endl;
cout <<" history.back(); </script>";

}
