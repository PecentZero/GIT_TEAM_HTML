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

string trim_space(string);
string make_filename (sql::Connection *con,string temp_filename);

int main() {
	Cgicc formData;
	string author_id, content_title,content_text,content_img;
	string path ="uploads/";
	cout << "Content-type :text/html\r\n\r\n";
	cout <<"<html>\n";
	cout <<"<head>\n";
	cout<<"<title>write</title>\n";
	cout <<"</head>\n";
	cout << "<body>\n";

	form_iterator f_title = formData.getElement("title"); //get title element
	form_iterator f_description = formData.getElement("description"); //get description element
	const_file_iterator f_file = formData.getFile("myfile"); // get myfile element


	sql::Driver *driver;
  sql::Connection *con;
	sql::PreparedStatement *pstmt;
  driver = get_driver_instance();
  con = driver->connect("localhost","root","root");
  con->setSchema("HTML_DB");


  content_title = trim_space(**f_title);
	content_text  = **f_description;
	content_img = make_filename(con,f_file->getFilename());


	string sql ="INSERT INTO post_content (content_title,content_text,content_img,date_created,time_written) VALUES (?,?,?,CURDATE(),CURTIME())";
	pstmt= con->prepareStatement(sql);
	pstmt->setString(1,path+content_title);
	pstmt->setString(2,content_text);
	pstmt->setString(2,content_img);
	pstmt->executeUpdate();

	delete pstmt;
	delete con;

	cout << "<script> alert(\"success\");" << endl;
	cout << "location.href=\"/index.html\" </script>";
	cout << "<br>\n";
	cout <<"</body>\n";
	cout <<"</html>\n";
 return 0;


}

string make_filename (sql::Connection *con,string temp_filename)  // check If there is same file name
{

	string filename=temp_filename;
	sql::ResultSet *res;
	sql::PreparedStatement *pstmt;


	while(1){
	string sql ="SELECT * from post_content where content_img = ?";
  pstmt= con->prepareStatement(sql);
  pstmt->setString(1,filename);
  res = pstmt->executeQuery();
  if(!(res->next())) //  file name is unique
	break;
	srand(time(NULL));
	filename += (rand()%10);
}

delete pstmt;
delete res;
return filename;
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
