#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

using namespace std;
using namespace cgicc;
// registration parameter
// Username , Email , Password , ConfirmPassword


Cgicc formData;
void Check_Element(form_iterator &f,string name); // print parameter's value
void trim_space(string &temp) // trim  front , back  space

int main()
{
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

  trim_space(f_Username);
  trim_space(f_Email);
  trim_space(f_Password);
  trim_space(f_ConfirmPassword);

	Check_Element(f_Username,string("Username"));
	Check_Element(f_Email,string("Email"));
	Check_Element(f_Password,string("Password"));
	Check_Element(f_ConfirmPassword,string("ConfirmPassword"));

	cout << "<br>\n";
	cout <<"</body>\n";
	cout <<"</html>\n";
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


void trim_space(string &temp)
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
}
