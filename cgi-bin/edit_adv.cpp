
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <cgicc/CgiDefs.h>  //for cgicc programming
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
bool load_adv(string adv_id,string &content_title,string &content_text,string &content_img,string &location);
void option_selected(string,string);
bool Check_auth(string session_value,string &username);
bool Check_adv_auth(string adv_id,string session_username);
string trim_space(string temp);

string alert_msg = "Success";
int main()
{
	string adv_id, session_value,session_name,username;
  string content_title,content_link,content_img, location;
	session_name="session_id";
	string global_path = "/";
  form_iterator f_adv_id = formData.getElement("adv_id"); //get adv_id element

  cout << "Content-type:text/html\r\n\r\n";
  cout << "<html>\n";
  cout << "\n";
  cout << "<head>\n";
  cout << "\n";
  cout << "<link href=\"../../vendors/bootstrap/css/bootstrap.min.css\" rel=\"stylesheet\">\n";
  cout << "<link href=\"../../css/style_post_create.css\" rel=\"stylesheet\">\n";
  cout << " <link rel=\"stylesheet\" href=\"../../css/bootstrap.css\">\n";
  cout << "        <link rel=\"stylesheet\" href=\"../../vendors/linericon/style.css\">\n";
  cout << "        <link rel=\"stylesheet\" href=\"../../css/font-awesome.min.css\">\n";
  cout << "        <link rel=\"stylesheet\" href=\"../../vendors/lightbox/simpleLightbox.css\">\n";
  cout << "        <link rel=\"stylesheet\" href=\"../../vendors/nice-select/css/nice-select.css\">\n";
  cout << "        <!-- main css -->\n";
  cout << "        <link rel=\"stylesheet\" href=\"../../css/style.css\">\n";
  cout << "        <link rel=\"stylesheet\" href=\"../../css/responsive.css\">\n";
  cout << "</head>\n";
  cout << "<body>\n";

  if(get_cookie_value(formData.getEnvironment(),session_name,session_value)
&&   Check_Element(f_adv_id,"adv_id") && Check_auth(session_value,username) && Check_adv_auth((**f_adv_id),username))// exist session cookie ,a dv id
	{
 		load_adv(**f_adv_id,content_title,content_link,content_img,location);
		adv_id = trim_space(**f_adv_id);

  cout << "<div class=\"container\">\n";
  cout << "	<div class=\"row\">\n";
  cout << "	    \n";
  cout << "	    <div class=\"col-md-8 col-md-offset-2\">\n";
  cout << "	        \n";
  cout << "    		<h1>Edit adv</h1>\n";
  cout << "    		\n";
  cout << "    		<form enctype =\"multipart/form-data\" action=\"create_adv.cgi?adv_id="<<trim_space(adv_id)<<"&type=update&\" method=\"POST\">\n";
  cout << "    		    \n";
  cout << "    		    \n";
  cout << "    		    \n";
  cout << "    		    <div class=\"form-group\">\n";
  cout << "    		        <label for=\"title\">Title <span class=\"require\">*</span></label>\n";
  cout << "    		        <input type=\"text\" value=\""<<content_title<<"\" class=\"form-control\" name=\"title\" />\n";
  cout << "    		    </div>\n";
  cout << "    		   \n";
  cout << "                <div class=\"form-group\">\n";
  cout << "                    <label for=\"location\">Location<span class=\"require\">*</span></label>\n";
  cout << "                    <select class=\"form-control\" name=\"location\">\n";
	option_selected("SEOUL",location);
	option_selected("BUSAN",location);
	option_selected("DAJEON",location);
  cout << "                    </select>\n";
  cout << "                </div>\n";
  cout << "\n";
  cout << "\n";
  cout << "    		    <div class=\"form-group\">\n";
  cout << "								<label for=\"link\">link</label>\n";
  cout << "    		        <textarea rows=\"5\" class=\"form-control\" name=\"link\" >" <<content_link << "</textarea>\n";
  cout << "    		    </div>\n";
  cout << "    		    \n";
  cout << "    		    \n";
  cout << "    		    <div>\n";
  cout << "    		    	<input type=\"file\" id=\"myfile\" name=\"myfile\"><br><br>\n";
  cout << "    		    </div>\n";

  if(!content_img.empty()) {
  cout << "    		    <div class=\"form-group\">\n";
	cout << "           <button type=\"button\" class=\"btn btn-info\" onclick = window.open(\""<< global_path + content_img<<"\")>view attached_file</button>\n";
	cout << "    		    \n";
	//cout << "           <button type=\"button\" class=\"btn btn-danger\" onclick = window.open(\"./delete_adv.cgi?adv_id"<<'='<<adv_id<<"&type=delfile"<<"\")>Delete_file</button>\n";
	cout << "           <button type=\"button\" class=\"btn btn-danger\" onclick = window.open(\"./delete_adv.cgi?adv_id"<<'='<<trim_space(adv_id)<<"&type=delfile"<<"\",\"_self\")>Delete_file</button>\n";
	cout << "    		    \n";
	cout << "    		    </div>\n";
}

  cout << "    		    <div class=\"form-group\">\n";
  cout << "    		        <button type=\"submit\" class=\"btn btn-primary\">\n";
  cout << "    		            Edit\n";
  cout << "    		        </button>\n";
  cout << "    		        <button class=\"btn btn-default\">\n";
  cout << "    		            Cancel\n";
  cout << "    		        </button>\n";
  cout << "    		    </div>\n";
  cout << "    		    \n";
  cout << "    		</form>\n";
  cout << "		</div>\n";
  cout << "		\n";
  cout << "	</div>\n";
  cout << "</div>\n";
  cout << "\n";
	}

	cout << "<script> alert(\""<<alert_msg<<"\");\n";
	cout << "</script>\n";
	cout << "</body>\n";
	cout << "\n";
	cout << "<!-- <script src=\"//maxcdn.bootstrapcdn.com/bootstrap/3.3.0/js/bootstrap.min.js\"></script>\n";
	cout << "<script src=\"//code.jquery.com/jquery-1.11.1.min.js\"></script> -->\n";
	cout << "</html>\n";


return 0;

}

void option_selected(string value,string check_value)
{

	if(value == check_value)
   cout << "<option value=\""<<value<<"\" selected >"<<value<<"</option>\n";
	else
	 cout << "<option value=\""<<value<<"\" >"<<value<<"</option>\n";
}

bool Check_Element(form_iterator &f,string msg) // print parameter's value
{ //also needed check space or valid value
	if(!f->isEmpty() && f != (*formData).end()) return true;
	else {
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
bool load_adv(string adv_id,string &content_title,string &content_link,string &content_img,string &location)
{//return img_content from adv content

	string globalpath= "../../";
	string alert_msg;

	sql::Driver *driver;
	sql::Connection *con;
	sql::ResultSet *res;
	sql::PreparedStatement *pstmt;

	driver = get_driver_instance();
	con = driver->connect("localhost","root","root");
	con->setSchema("HTML_DB");


	string sql ="SELECT content_title,content_link,content_img,location from adv_content where adv_id = ?";//delete record
	pstmt= con->prepareStatement(sql);
	pstmt->setString(1,adv_id);
  res = pstmt->executeQuery();
	delete pstmt;
  if(res->next()){
  content_title = res->getString("content_text");
  content_link  = res->getString("content_link");
  content_img = res->getString("content_img");
  location = res->getString("location");
  delete res;
	delete con;
	return true; //success loading
  }

delete con;
return false; //fail loaing
}

string trim_space(string temp)
{

   int len,end;
   int start_end, end_start, i;
   len = temp.length();
   for (i = 0; i<len; i++)
   {
       if (temp[i] != ' ') break;    // i-1
   }
   start_end = i - 1;
   if (start_end >= 0)
       temp.replace(0, start_end + 1, "");

   len = temp.length();
   end = len;
   for (i = len - 1; i >= 0; i--)
   {
       if (temp[i] != ' ') break;  // i+1
   }
   end_start = i + 1;
   if (end_start < len)
   temp.replace(end_start, end - end_start, "");
   return temp;

}
