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

using namespace std;
using namespace sql;

char* generate_random_string(int length);
int check_overlap(char* random_string, sql::Connection *con);
char* generate_cookie(int length, sql::Connection *con);
char* session_check(char* cookie, sql::Connection *con);
void create_session(char* cookie, char* username, sql::Connection *con);

char* session_check(char* cookie, sql::Connection *con)
{
	sql::ResultSet *res;
	sql::PreparedStatement *pstmt;

	string sql = "SELECT * FROM session where cookie = ?";
	pstmt = con->prepareStatement(sql);
	pstmt->setString(1, cookie);
	res = pstmt->executeQuery();

	if (res->next())
	{
		char* result = strdup((res->getString("username")).c_str());
		delete res;
		delete pstmt;
		return result;
	}
	else
	{
		delete res;
		delete pstmt;
		return NULL;
	}
}

void create_session(char* cookie, char* username, sql::Connection *con)
{
	sql::PreparedStatement *pstmt;

	string sql = "INSERT INTO session(username, cookie) VALUES (?, ?)";
	pstmt = con->prepareStatement(sql);
	pstmt->setString(1, username);
	pstmt->setString(2, cookie);
	pstmt->execute();
	delete pstmt;

	return;
}

char *generate_random_string(int length)
{
	char* charList = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	srand(time(NULL));
	char* result = (char*) malloc(sizeof(char) * length + 1);
	for (int i = 0; i < length; i++){
		result[i] = charList[rand() % 62];
	}
	result[length] = '\0';

	return result;
}

int check_overlap(char* random_string, sql::Connection *con)
{
	sql::ResultSet *res;
	sql::PreparedStatement *pstmt;

	string sql = "SELECT * FROM session WHERE cookie = ?";
	pstmt = con->prepareStatement(sql);
	pstmt->setString(1, random_string);
	res = pstmt->executeQuery();

	if (res->next())
	{
		delete pstmt;
		delete res;
		return 1;
	}
	delete pstmt;
	delete res;
	return 0;
}

char *generate_cookie(int length, sql::Connection *con)
{
	while (1)
	{
		char* random_string = generate_random_string(length);
		if (check_overlap(random_string, con)){
			free(random_string);
			continue;
		}
		else {
			return random_string;
		}
	}
}
