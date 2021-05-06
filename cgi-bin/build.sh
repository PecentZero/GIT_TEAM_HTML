#!/bin/sh

cd /var/www/cgi-bin/

rm *.cgi
g++ -o login.cgi login.cpp cookie.cpp -lmysqlcppconn -lcgicc
g++ -o logout.cgi logout.cpp -lmysqlcppconn -lcgicc
g++ -o create_post.cgi create_post.cpp -lmysqlcppconn -lcgicc
g++ -o index.cgi index.cpp -lmysqlcppconn -lcgicc
g++ -o index.cgi index.cpp cookie.cpp -lmysqlcppconn -lcgicc
g++ -o register.cgi register.cpp -lmysqlcppconn -lcgicc
g++ -o edit_post.cgi edit_post.cpp -lmysqlcppconn -lcgicc
g++ -o delete_post.cgi delete_post.cpp -lmysqlcppconn -lcgicc
g++ -o settings.cgi settings.cpp cookie.cpp -lmysqlcppconn -lcgicc
g++ -o create_adv.cgi create_adv.cpp -lmysqlcppconn -lcgicc
