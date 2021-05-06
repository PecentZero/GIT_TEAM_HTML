#pragma once
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string.h>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

char* generate_random_string(int length);
int check_overlap(char* random_string, sql::Connection *con);
char* generate_cookie(int length, sql::Connection *con);
char* session_check(char* cookie, sql::Connection *con);
void create_session(char* cookie, char* username, sql::Connection *con);

