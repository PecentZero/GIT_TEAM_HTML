
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

bool Check_Element(form_iterator &f);
bool get_cookie_value(const CgiEnvironment &env,string cookie_name, string &wanted_value);
void load_post(string session_cookie,string post_id,string &content_title,string &content_text,string &content_img,string &location);
int main()
{
	string post_id, session_value,session_cookie;
  string content_title,content_text,content_img, location;
  string global_path = "/";
  form_iterator f_post_id = formData.getElement("post_id"); //get post_id element


  cout << "Content-type:text/html\r\n\r\n";
  cout << "<html>\n";
  cout << "\n";
  cout << "<head>\n";
  cout << "\n";
  cout << "<link href=\"../../vendor/bootstrap/css/bootstrap.min.css\" rel=\"stylesheet\">\n";
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

  if(get_cookie_value(formData.getEnvironment(),string("session"),session_cookie)
&&   Check_Element(f_post_id))// exist session cookie , post id
	{

	 load_post(session_cookie,**f_post_id,content_title,content_text,content_img,location);//from DB file fetching

  cout << "<div class=\"container\">\n";
  cout << "	<div class=\"row\">\n";
  cout << "	    \n";
  cout << "	    <div class=\"col-md-8 col-md-offset-2\">\n";
  cout << "	        \n";
  cout << "    		<h1>Edit post</h1>\n";
  cout << "    		\n";
  cout << "    		<form action=\"writer.cgi\" method=\"POST\">\n";
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
  cout << "                      <option value=\"SEOUL\">SEOUL</option>\n";
  cout << "                      <option value=\"BUSAN\">BUSAN</option>\n";
  cout << "                      <option value=\"DAJEON\">DAJEON</option>\n";
  cout << "                    </select>\n";
  cout << "                </div>\n";
  cout << "\n";
  cout << "\n";
  cout << "    		    <div class=\"form-group\">\n";
  cout << "    		        <label for=\"description\">Description</label>\n";
  cout << "    		        <textarea rows=\"5\" class=\"form-control\" name=\"description\" >" <<content_text << "</textarea>\n";
  cout << "    		    </div>\n";
  cout << "    		    \n";
  cout << "    		    \n";
  cout << "    		    <div>\n";
  cout << "    		    	<input type=\"file\" id=\"myfile\" name=\"myfile\"><br><br>\n";
  cout << "    		    </div>\n";
/*
  if(content_img != string(NULL))  {
  cout << "    		    <div class=\"form-group\">\n";
  cout << "           <button class=\"btn btn-info\" onclick = window.open(\""<< global_path + content_img<<"\")>view attached_file</button>\n";
  cout << "    		    \n";
  cout << "    		    </div>\n";
}
*/
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

  else{
	cout << "<script> alert(\"Missing post_id or session\");\n" <<endl;
	cout <<" history.back(); </script>\n";
  }
  cout << "</body>\n";
  cout << "\n";
  cout << "<!-- <script src=\"//maxcdn.bootstrapcdn.com/bootstrap/3.3.0/js/bootstrap.min.js\"></script>\n";
  cout << "<script src=\"//code.jquery.com/jquery-1.11.1.min.js\"></script> -->\n";
  cout << "</html>\n";


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

void load_post(string session_cookie,string post_id,string &content_title,string &content_text,string &content_img,string &location)
{//return img_content from post_content

	string session_username, post_username;
	string globalpath= "../../";
	string alert_msg;

//try{
	sql::Driver *driver;
	sql::Connection *con;
	driver = get_driver_instance();
	con = driver->connect("localhost","root","root");
	con->setSchema("HTML_DB");
  sql::ResultSet *res;
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

	 string sql ="SELECT author_id from post_content where post_id = ?";
	 pstmt= con->prepareStatement(sql);
	 pstmt->setString(1,post_id);
   res_post_content = pstmt->executeQuery();

			 if(res_post_content->next()) // post exist
			 {

			      post_username = res_post_content->getString("author_id");

						delete res_post_content;
						delete pstmt;

								if(session_username == post_username) //authentication
										{

											string sql ="SELECT content_title,content_text,content_img,location from post_content where post_id = ?";//delete record
											pstmt= con->prepareStatement(sql);
											pstmt->setString(1,post_id);
										  res = pstmt->executeQuery();
											delete pstmt;
                      if(res->next()){
                      content_title = res->getString("content_text");
                      content_text  = res->getString("content_text");
                      content_img = res->getString("content_img");
                      location = res->getString("location");
                      delete res;
                      alert_msg ="Success loading";
                      }
                    }

									else alert_msg = "Permission Error"; 	//cout << "don't have permission"
			 }
        else alert_msg = "No matching post";//cout << "no matching post"

     }

else alert_msg = "no matching session";//cout << "no matching session";

delete con;

/*
}catch (sql::SQLException &e) {
     cout << "# ERR: SQLException in " << __FILE__;
     cout << "(" << __FUNCTION__ << ") on line >> " << __LINE__ << endl;
     cout << "# ERR: " << e.what();
     cout << " (MySQL error code: " << e.getErrorCode();
     cout << ", SQLState: " << e.getSQLState() <<">> "<< " )" << endl;
}
*/

cout << "<script> alert(\""<<alert_msg<<"\");\n" <<endl;
cout <<"</script>\n"<<endl;


}
