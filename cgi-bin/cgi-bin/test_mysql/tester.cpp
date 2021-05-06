#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#define EXAMPLE_HOST "localhost"
#define EXAMPLE_USER "yourusername"
#define EXAMPLE_PASS "yourpassword"
#define EXAMPLE_DB "yourdatabase"
using namespace std;
int main(int argc, const char **argv)
{
const string url=EXAMPLE_HOST;
const string user=EXAMPLE_USER;
const string pass=EXAMPLE_PASS;
const string database=EXAMPLE_DB;
sql::Driver* driver = get_driver_instance();
std::auto_ptr<sql::Connection> con(driver->connect(url, user, pass));
con->setSchema(database);
return 0;
}
