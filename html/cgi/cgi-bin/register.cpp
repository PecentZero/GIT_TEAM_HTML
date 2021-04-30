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
void Check_Element(form_iterator &f,const std::string name); // print parameter's value

int main()
{
   	cout << "<html>\n";
   	cout << "<head>\n";
  	cout << "<title>Text Area Data to CGI</title>\n";
 	cout << "</head>\n";
  	cout << "<body>\n";

	form_iterator f_Username = formData.getElement("Username"); // handling parameter getting by post method
	form_iterator f_Email = formData.getElement("Email");
	form_iterator f_Password = formData.getElement("Password");
	form_iterator f_ConfirmPassword = formData.getElement("ConfirmPassword");

	Check_Element(f_Username,string("Username"));
	Check_Element(f_Email,string("Email"));
	Check_Element(f_Password,string("Password"));
	Check_Element(f_ConfirmPassword,string("ConfirmPassword"));
	
	cout << "<br>\n";
	cout <<"</body>\n";
	cout <<"</html>\n";
	return 0;
}

void Check_Element(form_iterator &f,const std::string name) // print parameter's value
{ //also needed check space or valid value
	if(!f->isEmpty() && f != (*formData).end()){
	cout<<"<<p>"<< name <<":" <<**f<<"</p>"<<endl;
	}else{
	cout <<"NO" <<name <<"entered" <<endl;
	}
}
