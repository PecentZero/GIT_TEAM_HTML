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

int main () {
   Cgicc formData;
  
   cout << "Content-type:text/html\r\n\r\n";
   cout << "<html>\n";
   cout << "<head>\n";
   cout << "<title>Testing login</title>\n";
   cout << "</head>\n";
   cout << "<body>\n";

   form_iterator f_id = formData.getElement("Username");  
   if( !f_id->isEmpty() && f_id != (*formData).end()) {  
      cout << "id : " << **f_id << endl;  
   } else {
      cout << "none id input" << endl;  
   }
   cout << "<br/>\n";
   cout << "</body>\n";
   cout << "</html>\n";
   
   return 0;
}
