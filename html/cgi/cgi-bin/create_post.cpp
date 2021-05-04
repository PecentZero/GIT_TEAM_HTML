#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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


bool Check_Element(form_iterator &);
bool Check_file_Element(const_file_iterator &);
string trim_space(string);
string make_filename (sql::Connection *con,string temp_filename);
void Insert_DB(form_iterator &, form_iterator &, form_iterator &, const_file_iterator &,string);
void Update_DB(form_iterator &,form_iterator &, form_iterator &, form_iterator &, const_file_iterator &);
bool get_cookie_value(const CgiEnvironment &,string, string &);
bool Check_auth(string,string,string&);

Cgicc formData;

int main() {
	string author_id,location, content_title,content_text,content_img;
	string session_name,session_value,user_name;
	session_name="session_id";

  cout << "Content-type:text/html\r\n\r\n";
	cout <<"<html>\n";
	cout <<"<head>\n";
	cout<<"<title>write</title>\n";
	cout <<"</head>\n";
	cout << "<body>\n";


	get_cookie_value(formData.getEnvironment(),session_name,session_value);
	form_iterator f_title = formData.getElement("title"); //get title element
	form_iterator f_location = formData.getElement("location"); //get location element
	form_iterator f_description = formData.getElement("description"); //get description element
	const_file_iterator f_file = formData.getFile("myfile"); // get myfile element

  form_iterator f_type = formData.getElement("type"); // for edit
	form_iterator f_post_id = formData.getElement("post_id"); //get post_id element

	if (Check_Element(f_title) &&  Check_Element(f_description) && Check_Element(f_location))//&& Check_file_Element(f_file)) // exist
{

	if(Check_Element(f_type) && **f_type == string("update") && Check_Element(f_post_id) && Check_auth(**f_post_id,session_value,user_name))
	{

		Update_DB(f_post_id,f_title,f_location,f_description,f_file);
	}
	else{
	Insert_DB(f_title,f_location,f_description,f_file,user_name);
}
	cout << "<script> alert(\"Success\");\n" << endl;
	cout << "location.href=\"../index.html\"; </script>\n"<<endl;
}
else
{
	cout << "<script> alert(\"Enter title ,location, contents  \");\n" << endl;
	cout << "history.back(); </script>\n"<<endl;

}


cout << "<br>\n";
cout <<"</body>\n";
cout <<"</html>\n";

 return 0;


}

bool Check_Element(form_iterator &f) //	get_cookie_value(formData.getEnvironment(),session_name,session_value);
 //print parameter's value
{ //also needed check space or valid value
	if(!f->isEmpty() && f != (*formData).end()) return true;
	else return false;
}

bool Check_file_Element(const_file_iterator &f) // print parameter's value
{ //also needed check space or valid value
	if(f!= formData.getFiles().end()) return true;
	else return false;
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


string make_filename (sql::Connection *con,string temp_filename)  // check If there is same file name
{

  string path ="uploads/";
	string filename=temp_filename;
	sql::ResultSet *res;
	sql::PreparedStatement *pstmt;
	string number="0123456789";

//if(filename == "NULL") filename = "NU";

	while(1){
	string sql ="SELECT * from post_content where content_img = ?";
  pstmt= con->prepareStatement(sql);
  pstmt->setString(1,path+filename);
  res = pstmt->executeQuery();
  if(!(res->next())) //  file name is unique
	break;
	srand(time(NULL));
	filename += number[rand()%10];
	delete res;
}

delete pstmt;
delete res;
return filename;
}



void Insert_DB(form_iterator &f_title , form_iterator &f_location ,form_iterator &f_description , const_file_iterator &f_file,string user_name)
{
	string content_title, content_text,content_img,location;
	string path ="uploads/";
  string globalpath ="../../";
	sql::Driver *driver;
	sql::Connection *con;
	sql::PreparedStatement *pstmt;
	driver = get_driver_instance();
	con = driver->connect("localhost","root","root");
	con->setSchema("HTML_DB");


	content_title = trim_space(**f_title);
	content_text  = **f_description;
	location = **f_location;

if(Check_file_Element(f_file)){  //file attachement exists

	content_img = make_filename(con,f_file->getFilename());

	ofstream f_o((globalpath+path+content_img).c_str(),ios::out|ios::binary);
	f_file->writeToStream(f_o);
	f_o.close();


	string sql ="INSERT INTO post_content (author_id,content_title,content_text,content_img,time_written,location) VALUES (?,?,?,?,NOW(),?)";
	pstmt= con->prepareStatement(sql);
	pstmt->setString(1,user_name);
	pstmt->setString(2,content_title);
	pstmt->setString(3,content_text);
	pstmt->setString(4,path+content_img);
	pstmt->setString(5,location);
	pstmt->executeUpdate();

	delete pstmt;
}

else{ //file attachement doesnot exist
	string sql ="INSERT INTO post_content (author_id,content_title,content_text,time_written,location) VALUES (?,?,?,NOW(),?)";
	pstmt= con->prepareStatement(sql);
	pstmt->setString(1,user_name);
	pstmt->setString(2,content_title);
	pstmt->setString(3,content_text);
	pstmt->setString(4,location);
	pstmt->executeUpdate();
		delete pstmt;
}

	delete con;



}


void Update_DB(form_iterator &f_post_id ,form_iterator &f_title , form_iterator &f_location ,form_iterator &f_description , const_file_iterator &f_file)
{
	string content_title, content_text,content_img,location,post_id;
	string delete_img; // should delete
	string path ="uploads/";
  string globalpath ="../../";
	sql::Driver *driver;
	sql::Connection *con;
	sql::PreparedStatement *pstmt;
	sql::ResultSet *res;
	driver = get_driver_instance();
	con = driver->connect("localhost","root","root");
	con->setSchema("HTML_DB");

  post_id = **f_post_id;
	content_title = trim_space(**f_title);
	content_text  = **f_description;
	location = **f_location;

if(Check_file_Element(f_file)){  //file modify

	content_img = make_filename(con,f_file->getFilename());


	ofstream f_o((globalpath+path+content_img).c_str(),ios::out|ios::binary);
	f_file->writeToStream(f_o);
	f_o.close();

	//shoulld delete prior image if exists

	string sql ="select content_img from post_content where post_id = ?";
	pstmt = con->prepareStatement(sql);
	pstmt->setString(1,post_id);
	res = pstmt->executeQuery();
	delete_img = res->getString("content_img");
	delete pstmt;
	delete res;
	if(delete_img != string("NULL")) // prior file exists so should delete
	remove((globalpath+delete_img).c_str()); //delete img file


	sql = "Update post_content set content_title = ? ,content_text = ?,content_img = ? ,time_written = NOW(),location = ? where post_id = ?";
	pstmt = con->prepareStatement(sql);
	pstmt->setString(1,content_title);
	pstmt->setString(2,content_text);
	pstmt->setString(3,path+content_img);
	pstmt->setString(4,location);
	pstmt->setString(5,post_id);
	pstmt->executeUpdate();

	delete pstmt;
}

else{ //keep prior file state
	string sql = "Update post_content set content_title = ?,content_text = ?,time_written = NOW(),location = ? where post_id = ?";
	pstmt = con->prepareStatement(sql);
	pstmt->setString(1,content_title);
	pstmt->setString(2,content_text);
	pstmt->setString(3,location);
	pstmt->setString(4,post_id);
	pstmt->executeUpdate();
		delete pstmt;
}

	delete con;

}


bool Check_auth(string post_id,string session_value,string &user_name)
{

		string post_username,session_username;

		sql::Driver *driver;
		sql::Connection *con;
		sql::ResultSet *res_session;
		sql::ResultSet *res_post_content;
		sql::PreparedStatement *pstmt;

		driver = get_driver_instance();
		con = driver->connect("localhost","root","root");
		con->setSchema("HTML_DB");

		//session
		string sql ="SELECT username from session where cookie = ?";
		pstmt= con->prepareStatement(sql);
		pstmt->setString(1,session_value);
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
										delete con;
										user_name = session_username;
									return true;
								}
						}

		}
		delete con;
		return false;
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
