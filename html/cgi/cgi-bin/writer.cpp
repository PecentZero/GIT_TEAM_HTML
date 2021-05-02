#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <cgicc/cgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeaer.h>
#include <cgicc/HTMLClasses.h>

using namespace std;
using namespace cgicc;

int main() {
	Cgicc formData;
	cout << "Content-type :text/html\r\n\r\n";
	cout <<"<html>\n";
	cout <<"<head>\n";
	cout<<"<title>write</title>\n";
	cout <<"</head>\n";
	cout << "<body>\n";

	form_iterator f_title = formData.getElemenet("title"); //get title element
	form_iterator f_description = formData.getElement("description"); //get description element
	const_file_iterator f_file = formData.getFile("myfile"); // get myfile element

        trim_dat




 
