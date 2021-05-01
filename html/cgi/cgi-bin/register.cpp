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
// registration parameter
// Username , Email , Password , ConfirmPassword


Cgicc formData;
void Check_Element(form_iterator &f,string name); // print parameter's value
string  trim_space(string);// trim  front , back  space

int main()
{
   string username,password,email;
   int option ; // option = 1  means success option = 0 means fail

    cout << "Content-type:text/html\r\n\r\n";
   	cout << "<html>\n";
   	cout << "<head>\n";
  	cout << "<title>Testing Registration</title>\n";
 	  cout << "</head>\n";
  	cout << "<body>\n";

	form_iterator f_Username = formData.getElement("Username"); // handling parameter getting by post method
	form_iterator f_Email = formData.getElement("Email");
	form_iterator f_Password = formData.getElement("Password");
	form_iterator f_ConfirmPassword = formData.getElement("ConfirmPassword");

	if(**f_Password != **f_ConfirmPassword) //alert error message and back to prior page
	{
		cout << "<script> alert(\"mismatch Password, ConfirmPassword\");" <<endl;
		cout <<" history.back(); </script>";
	}
	

 	
	else{
	username = trim_space(**f_Username);
 	email = trim_space(**f_Email);
 	 password = trim_space(**f_Password);

 try{
   sql::Driver *driver;
   sql::Connection *con;
   sql::Statement *stmt;
   sql::ResultSet *res;
   sql::PreparedStatement *pstmt;

   driver = get_driver_instance();
   con = driver->connect("localhost","root","root");
   con->setSchema("HTML_DB");
  
   string sql ="SELECT * from user_auth where username = ?";
   pstmt= con->prepareStatement(sql);
   pstmt->setString(1,username);
   res = pstmt->executeUpdate();
   if(!res) // means there already exists same username
   {
     
	delete res;
	delete pstmt;
	cout << "<script> alert(\"There already exists same Username\");" <<endl;
	cout <<" history.back(); </script>";
   }
   else{
   string sql ="SELECT * from user_profile  where email = ?";
    pstmt= con->prepareStatement(sql);
   pstmt->setString(1,email);
   res = pstmt->executeUpdate();
   if(!res) // means there already exists same username
   {
     
	delete res;
	delete pstmt;
	cout << "<script> alert(\"There already exists same email\");" <<endl;
	cout <<" history.back(); </script>";
   }
else{
   
   string sql ="INSERT INTO user_auth(username,password) VALUES (?,SHA2(?,512))";
   pstmt= con->prepareStatement(sql);
   pstmt->setString(1,username);
   pstmt->setString(2,password);
   pstmt->executeUpdate();

   sql ="INSERT INTO user_profile(username,email) VALUES (?,?)";
   pstmt=con->prepareStatement(sql);
   pstmt->setString(1,username);
   pstmt->setString(2,email);
   pstmt->executeUpdate();
   delete pstmt;

 }catch (sql::SQLException &e) {
  cout << "# ERR: SQLException in " << __FILE__;
  cout << "(" << __FUNCTION__ << ") on line >> " << __LINE__ << endl;
  cout << "# ERR: " << e.what();
  cout << " (MySQL error code: " << e.getErrorCode();
  cout << ", SQLState: " << e.getSQLState() <<">> "<< " )" << endl;
}
///success registration
 cout << "<script> alert(\"Success Registration\");"<<endl;
 cout << "location.href = \"../index.html\"</script>"<<endl;

	cout << "<br>\n";
	cout <<"</body>\n";

	cout <<"</html>\n";


}
}
}
	return 0;
}

void Check_Element(form_iterator &f,string name) // print parameter's value
{ //also needed check space or valid value
	if(!f->isEmpty() && f != (*formData).end()){
	cout<<"<p>"<< name <<":" <<**f<<"</p>"<<endl;
	}else{
	cout <<"<p> NO "<<name <<" entered</p>" <<endl;
	}
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
