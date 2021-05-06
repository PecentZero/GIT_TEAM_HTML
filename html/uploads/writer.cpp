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
void Insert_DB(form_iterator &, form_iterator &, form_iterator &, const_file_iterator &);


Cgicc formData;
int main() {
	string author_id,location, content_title,content_text,content_img;
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


	if (Check_Element(f_title) &&  Check_Element(f_description) && Check_Element(f_location))//&& Check_file_Element(f_file)) // exist
{
	Insert_DB(f_title,f_location,f_description,f_file);
	cout << "<script> alert(\"Success\");" << endl;
	cout << "location.href=\"../index.html\"; </script>"<<endl;
}
else
{
	cout << "<script> alert(\"Enter title ,location, contents  \");" << endl;
	cout << "location.href=\"../index.html\"; </script>"<<endl;

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



void Insert_DB(form_iterator &f_title , form_iterator &f_location ,form_iterator &f_description , const_file_iterator &f_file)
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


	string sql ="INSERT INTO post_content (content_title,content_text,content_img,time_written,location) VALUES (?,?,?,NOW(),?)";
	pstmt= con->prepareStatement(sql);
	pstmt->setString(1,content_title);
	pstmt->setString(2,content_text);
	pstmt->setString(3,path+content_img);
	pstmt->setString(4,location);
	pstmt->executeUpdate();

	delete pstmt;
}

else{ //file attachement doesnot exist
	string sql ="INSERT INTO post_content (content_title,content_text,time_written,location) VALUES (?,?,NOW(),?)";
	pstmt= con->prepareStatement(sql);
	pstmt->setString(1,content_title);
	pstmt->setString(2,content_text);
	pstmt->setString(3,location);
	pstmt->executeUpdate();
		delete pstmt;
}

	delete con;



}