#define SQL_PASSWORD "root"

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


bool Check_Element(form_iterator &f,string msg);
bool Check_ElementN(form_iterator &f);
bool Check_file_Element(const_file_iterator &,string);
bool Check_img_fileN_Element(const_file_iterator &f,string msg); //img file or none file
bool Check_img_file_Element(const_file_iterator &f,string msg);
string trim_space(string);
string make_filename (sql::Connection *con,string temp_filename);
void Insert_DB(form_iterator &, form_iterator &, form_iterator &, const_file_iterator &,string);
void Update_DB(form_iterator &,form_iterator &, form_iterator &, form_iterator &, const_file_iterator &);
bool get_cookie_value(const CgiEnvironment &,string, string &);
bool Check_auth(string session_value,string &username);
bool Check_post_auth(string post_id,string session_username);


Cgicc formData;

string alert_msg ="Success";

void exploit()
{
	cout << "[TEAM HTML] Dummy Function for PoC\n";
}

int main() {
	string author_id,location, content_title,content_text,content_img;
	string session_name,session_value,username;
	int option = 1 ; // 0 => go to root home , 1 -> history.back()  3-> edit page
	session_name="session_id";

  cout << "Content-type:text/html\r\n\r\n";
	cout <<"<html>\n";
	cout <<"<head>\n";
	cout<<"<title>write</title>\n";
	cout <<"</head>\n";
	cout << "<body>\n";



	form_iterator f_title = formData.getElement("title"); //get title element
	form_iterator f_location = formData.getElement("location"); //get location element
	form_iterator f_description = formData.getElement("description"); //get description element
	const_file_iterator f_file = formData.getFile("myfile"); // get myfile element

	form_iterator f_post_id = formData.getElement("post_id"); //get post_id element
  form_iterator f_type = formData.getElement("type"); // for edit


	if (get_cookie_value(formData.getEnvironment(),session_name,session_value)&&Check_auth(session_value,username)&&Check_Element(f_title,"title") &&  Check_Element(f_description,"content") && Check_Element(f_location,"location")&&Check_img_fileN_Element(f_file,"img_file"))//&& Check_file_Element(f_file)) // exist
{

	if(Check_ElementN(f_type))
	{
		if( **f_type == string("update")){
			if(Check_Element(f_post_id,"post_id")&&Check_post_auth(**f_post_id,username))
			{ Update_DB(f_post_id,f_title,f_location,f_description,f_file);
				option=2;
		 }
			}
		else alert_msg ="type parameter error or post_id error";
		}

	else
	Insert_DB(f_title,f_location,f_description,f_file,username);
	option=0;
}

cout << "<script> alert(\""<<alert_msg<<"\");\n";
if(option == 0) // go root
cout << " location.href = \"/\"</script>\n";
if(option == 1) //go back
cout <<" history.back(); </script>\n";
if(option == 2) //go edit
cout <<" location.href = \"/cgi-bin/edit_post.cgi?post_id="<<trim_space(**f_post_id)<<"\" </script>\n";
else cout <<"</script>\n";


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
bool Check_ElementN(form_iterator &f) // print parameter's value
{ //also needed check space or valid value
	if(!f->isEmpty() && f != (*formData).end()) return true;
	else {
		return false;
	}
}

bool Check_file_Element(const_file_iterator &f,string msg)
{ //also needed check space or valid value
	if(f != formData.getFiles().end()) return true;
	else {
		return false;
	}
}


bool Check_img_fileN_Element(const_file_iterator &f,string msg)
{ //also needed check space or valid value
  string filetype;
	if(f != formData.getFiles().end()){
      filetype = f->getDataType();
      filetype = filetype.substr(0,5);

      if(filetype.length() >= 5 && filetype == "image")
        return true;
      else {alert_msg = "Not image file";
			return false;
			}
  }
	else true ; //No file ok
}


bool Check_img_file_Element(const_file_iterator &f,string msg)
{ //also needed check space or valid value
  string filetype;
	if(f != formData.getFiles().end()){
      filetype = f->getDataType();
      filetype = filetype.substr(0,5);

      if(filetype.length() >= 5 && filetype == "image")
        return true;
      else {alert_msg = "Not image file";
			return false;
			}
  }
else alert_msg = "image file must be attached!";
  return false;
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
	delete pstmt;
	delete res;
}

delete pstmt;
delete res;
return filename;
}



void Insert_DB(form_iterator &f_title , form_iterator &f_location ,form_iterator &f_description , const_file_iterator &f_file,string username)
{
	string content_title, content_text,content_img,location;
	string path ="uploads/";
  string globalpath ="../html/";
	sql::Driver *driver;
	sql::Connection *con;
	sql::PreparedStatement *pstmt;
	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306","root",SQL_PASSWORD);
	con->setSchema("HTML_DB");


	content_title = trim_space(**f_title);
	content_text  = **f_description;
	location = **f_location;

if(Check_file_Element(f_file,"file")){  //file attachement exists
	content_img = make_filename(con,f_file->getFilename());

	ofstream f_o((globalpath+path+content_img).c_str(),ios::out|ios::binary);
	f_file->writeToStream(f_o);
	f_o.close();


	string sql ="INSERT INTO post_content (author_id,content_title,content_text,content_img,time_written,location) VALUES (?,?,?,?,NOW(),?)";
	pstmt= con->prepareStatement(sql);
	pstmt->setString(1,username);
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
	pstmt->setString(1,username);
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
  string globalpath ="../html/";
	sql::Driver *driver;
	sql::Connection *con;
	sql::PreparedStatement *pstmt;
	sql::ResultSet *res;
	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306","root",SQL_PASSWORD);
	con->setSchema("HTML_DB");

  post_id = **f_post_id;
	content_title = trim_space(**f_title);
	content_text  = **f_description;
	location = **f_location;


if(Check_file_Element(f_file,"file")){  //file modify

	string sql ="select content_img from post_content where post_id = ?";
	pstmt = con->prepareStatement(sql);
	pstmt->setString(1,post_id);
	res = pstmt->executeQuery();
	res->next();
	delete_img = res->getString("content_img");
	delete pstmt;
	delete res;
	if(!delete_img.empty()) // prior file exists so should delete
	{
		remove((globalpath+delete_img).c_str()); //delete img file

		sql = "Update post_content set content_img = NULL ,time_written = NOW() where post_id = ?";
		pstmt = con->prepareStatement(sql);
		pstmt->setString(1,post_id);
		pstmt->executeUpdate();

		delete pstmt;
}

	content_img = make_filename(con,f_file->getFilename());


	ofstream f_o((globalpath+path+content_img).c_str(),ios::out|ios::binary);
	f_file->writeToStream(f_o);
	f_o.close();

	//shoulld delete prior image if exists




	sql = "Update post_content set content_title = ? ,content_text = ?,content_img = ? ,time_written = NOW(),location = ? where post_id = ?";
	pstmt = con->prepareStatement(sql);
	pstmt->setString(1,content_title);
	pstmt->setString(2,content_text);
	pstmt->setString(3,path+content_img);
	pstmt->setString(4,location);
	pstmt->setString(5,post_id);
	pstmt->executeUpdate();

	delete pstmt;

	cout << "<script> alert(\""<<"wow"<<"\");\n" <<endl;
	cout <<" location.href=\"/cgi-bin/edit_post.cgi?post_id="<<post_id<<"; </script>\n";


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




bool Check_auth(string session_value,string &username)
{

		sql::Driver *driver;
		sql::Connection *con;
		sql::ResultSet *res;
		sql::PreparedStatement *pstmt;

		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306","root",SQL_PASSWORD);
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
			delete con;
			return true;
		}
		delete res;
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
		con = driver->connect("localhost","root",SQL_PASSWORD);
		con->setSchema("HTML_DB");

		string sql ="SELECT author_id from post_content where post_id = ?";
		pstmt= con->prepareStatement(sql);
		pstmt->setString(1,post_id);
		res= pstmt->executeQuery();

				 if(res->next()) // post exist
				 {

							post_username = res->getString("author_id");

									if(session_username == post_username) //authentication
									{
										delete res;
										delete pstmt;
										delete con;
									return true;
								}
						}
					else alert_msg ="No Permission";

			delete res;
			delete pstmt;
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
		alert_msg = "Login First!";
    return false;
}
