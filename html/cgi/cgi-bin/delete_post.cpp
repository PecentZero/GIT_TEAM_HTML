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

Cgicc formData;
int main() {
	string author_id, content_title,content_text,content_img;
  cout << "Content-type:text/html\r\n\r\n";
	cout <<"<html>\n";
	cout <<"<head>\n";
	cout<<"<title>delete_post</title>\n";
	cout <<"</head>\n";
	cout << "<body>\n";


	form_iterator f_title = formData.getElement("post_id"); //get post_id element
	form_iterator f_description = formData.getElement("description"); //get description element
	const_file_iterator f_file = formData.getFile("myfile"); // get myfile element
